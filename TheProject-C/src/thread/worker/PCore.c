#include "ThreadPool.h"

#include <stdatomic.h>
#include <stdlib.h>
#include <threads.h>
#include <sched.h>
#include <pthread.h>
#include <immintrin.h>

// =============================================================================
// HARDWARE AFFINITY & L1/L2 CACHE OPTIMIZATION
// =============================================================================

/**
 * Bắt buộc gán thread P-Core vào đúng Core vật lý để tránh việc OS chuyển Thread
 * giữa các Core làm mất dữ liệu trong L1/L2 Cache.
 */
void pcore_set_thread_affinity(size_t thread_id) {
    if (thread_id >= TOTAL_HARDWARE_CORES) return; // Chỉ áp dụng cho các P-Core

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(thread_id, &cpuset);

    pthread_t current_thread = pthread_self();
    pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
}

// =============================================================================
// DIRECT DAG CONTINUATION ENGINE (CACHE LOCALITY)
// =============================================================================

/**
 * Thực thi một TaskHandle và ngay lập tức chạy luôn Task con nếu nó cũng là P-Core task.
 * Kỹ thuật này giữ cho Pipeline tính toán của P-Core không bị gián đoạn và tận dụng
 * triệt để L1/L2 Cache.
 */
void pcore_execute_direct_chain(TaskHandle* start_task) {
    TaskHandle* current = start_task;

    while (current != nullptr) {
        // Đánh dấu Task đang chạy
        atomic_store_explicit(&current->state, TASK_STATE_EXECUTING, memory_order_release);[cite: 2]

        void* result = nullptr;
        if (current->func != nullptr) {
            result = current->func(current, current->arg);[cite: 2]
        }

        if (current->future_result != nullptr) {
            atomic_store_explicit(current->future_result, result, memory_order_release);[cite: 2]
        }

        if (current->counter != nullptr) {
            atomic_fetch_sub_explicit(current->counter, 1, memory_order_release);[cite: 2]
        }

        atomic_store_explicit(&current->state, TASK_STATE_FINISHED, memory_order_release);[cite: 2]

        // Duyệt danh sách các node con trong DAG (Fan-out)
        DependencyNode* curr_dep = atomic_load_explicit(&current->dependents_head, memory_order_acquire);[cite: 2]
        TaskHandle* next_pcore_task = nullptr;

        while (curr_dep != nullptr) {
            TaskHandle* child = curr_dep->task;[cite: 2]

            // Giảm dependency count bằng thao tác atomic
            if (atomic_fetch_sub_explicit(&child->dependency_count, 1, memory_order_acq_rel) == 1) {[cite: 2]
                atomic_store_explicit(&child->state, TASK_STATE_READY, memory_order_release);[cite: 2]

                // Nếu node con cũng nhắm vào P-Core và chưa có node con nào được chọn làm next,
                // ta ưu tiên giữ node con này lại để chạy LẠI TRÊN CHÍNH THREAD NÀY!
                if (child->target_core == CORE_TYPE_PCORE && next_pcore_task == nullptr) {
                    next_pcore_task = child;
                } else {
                    // Nếu đã có task nối tiếp hoặc task dành cho E-Core -> đẩy vào Queue cho Thread khác
                    queue_push(current->pool, child->priority, child->target_core, child);[cite: 2]
                    cnd_signal(&current->pool->wake_signals);[cite: 2]
                }
            }
            curr_dep = curr_dep->next;[cite: 2]
        }

        if (current->parent != nullptr) {
            atomic_fetch_sub_explicit(&current->parent->active, 1, memory_order_release);[cite: 2]
        }

        // Thu hồi memory của task về Arena
        arena_free_task(current);[cite: 2]

        // Nhảy thẳng tới Task con mà không qua Scheduling Queue
        current = next_pcore_task;
    }
}

// =============================================================================
// BATCH SUBMIT & ADVANCED GRAPH PROCESSING
// =============================================================================

/**
 * Submit toàn bộ một DAG Graph vào hệ thống cùng lúc.
 * Chỉ các Root Node (dependency_count == 0) mới được đẩy trực tiếp vào MPMC Queue,
 * các Node còn lại sẽ tự động kích hoạt nối tiếp khi các Root Node hoàn thành.
 */
void pcore_dag_submit_batch(PCoreDAGGraph* graph, _Atomic(size_t)* counter) {
    if (graph == nullptr || graph->task_count == 0) return;

    for (size_t i = 0; i < graph->task_count; ++i) {
        TaskHandle* task = graph->tasks[i];

        if (counter != nullptr) {
            task->counter = counter;
            atomic_fetch_add_explicit(counter, 1, memory_order_relaxed);[cite: 1]
        }

        // Kiểm tra xem Node có phải là Root Node không
        if (atomic_load_explicit(&task->dependency_count, memory_order_acquire) == 0) {
            atomic_store_explicit(&task->state, TASK_STATE_READY, memory_order_release);[cite: 2]
            queue_push(graph->pool, task->priority, task->target_core, task);[cite: 2]
        }
    }

    // Đánh thức các P-Core Thread đang bận Spin hoặc Wait
    cnd_broadcast(&graph->pool->wake_signals);[cite: 1]
}

/**
 * Vòng lặp bận (Spin Engine) dành riêng cho P-Core Worker.
 * Giúp tránh gọi lệnh system call Futex làm giảm tần số xung nhịp của P-Core.
 */
TaskHandle* pcore_spin_fetch_task(Thrd* pool) {
    TaskHandle* task = nullptr;

    // Quét ưu tiên từ CRITICAL -> LOW dành cho P-Core
    for (int p = 0; p < TASK_PRIO_COUNT; ++p) {
        task = queue_pop(pool, (TaskPriority)p, CORE_TYPE_PCORE);[cite: 2]
        if (task != nullptr) return task;
    }

    // Nếu không có Task P-Core, thử đi Steal Task từ E-Core Queue
    for (int p = 0; p < TASK_PRIO_COUNT; ++p) {
        task = queue_pop(pool, (TaskPriority)p, CORE_TYPE_ECORE);[cite: 2]
        if (task != nullptr) return task;
    }

    return nullptr;
}

/**
 * Khởi tạo một DAG Graph chuyên dụng cho PCore
 */
PCoreDAGGraph* pcore_dag_create(Thrd* pool, size_t initial_capacity) {
    PCoreDAGGraph* graph = (PCoreDAGGraph*)malloc(sizeof(PCoreDAGGraph));
    if (!graph) return nullptr;

    graph->tasks = (TaskHandle**)malloc(sizeof(TaskHandle*) * initial_capacity);
    graph->task_count = 0;
    graph->capacity = initial_capacity;
    graph->pool = pool;
    return graph;
}

/**
 * Tạo một Node trong PCore DAG với cấu hình ép buộc vào P-Core và ưu tiên cao
 */
TaskHandle* pcore_dag_add_node(PCoreDAGGraph* graph, TaskFunc func, void* arg, TaskPriority prio) {
    // Ép buộc target_core luôn là CORE_TYPE_PCORE[cite: 1]
    TaskHandle* task = pool_create_task_ex(graph->pool, func, arg, prio, CORE_TYPE_PCORE);[cite: 1]
    if (!task) return nullptr;

    if (graph->task_count >= graph->capacity) {
        size_t new_cap = graph->capacity * 2;
        TaskHandle** new_tasks = (TaskHandle**)realloc(graph->tasks, sizeof(TaskHandle*) * new_cap);
        if (!new_tasks) return nullptr;
        graph->tasks = new_tasks;
        graph->capacity = new_cap;
    }

    graph->tasks[graph->task_count++] = task;
    return task;
}

/**
 * Thiết lập cạnh phụ thuộc: parent_task hoàn thành -> kích hoạt child_task
 */
PCORE_INLINE static bool pcore_dag_add_edge(TaskHandle* parent_task, TaskHandle* child_task) {
    return pool_add_dependency(parent_task, child_task);[cite: 1]
}

/**
 * Kích hoạt thực thi toàn bộ PCore DAG
 */
static inline void pcore_dag_submit(PCoreDAGGraph* graph, _Atomic(size_t)* counter) {
    for (size_t i = 0; i < graph->task_count; ++i) {
        pool_submit_task(graph->tasks[i], counter, nullptr);[cite: 1]
    }
}

/**
 * Giải phóng bộ nhớ của cấu trúc quản lý Graph (không hủy TaskHandle vì Arena tự thu hồi)
 */
static inline void pcore_dag_destroy(PCoreDAGGraph* graph) {
    if (!graph) return;
    if (graph->tasks) free(graph->tasks);
    free(graph);
}

/**
 * Active Spin bằng hardware hint dành riêng cho các đoạn chờ ngắn trên PCore
 */
PCORE_INLINE static void pcore_spin_pause(uint32_t iterations) {
    for (uint32_t i = 0; i < iterations; ++i) {
        _mm_pause(); // Giảm nhiệt độ & điện năng nhưng giữ Pipeline P-Core ấm
    }
}
