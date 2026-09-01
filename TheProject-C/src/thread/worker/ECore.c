#define _GNU_SOURCE
#include "ECore.h"

#include <stdlib.h>
#include <threads.h>
#include <pthread.h>
#include <sched.h>

// =============================================================================
// THREAD AFFINITY CHO E-CORE
// =============================================================================

void ecore_set_thread_affinity(size_t thread_id) {
    // Chỉ áp dụng cho E-Cores (Thread ID từ 16 đến 23 theo topology hệ thống của bạn)
    if (thread_id < 16 || thread_id >= TOTAL_HARDWARE_THREADS) return;[cite: 1, 2]

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(thread_id, &cpuset);

    pthread_t current_thread = pthread_self();
    pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
}

// =============================================================================
// CONTEXT MANAGEMENT & MEMORY MARSHALLING
// =============================================================================

ECoreFFIContext* ecore_ffi_create_context(void* java_in, void* java_out, size_t count, size_t elem_size) {
    if (java_in == nullptr || count == 0) return nullptr;

    ECoreFFIContext* ctx = (ECoreFFIContext*)malloc(sizeof(ECoreFFIContext));
    if (ctx == nullptr) return nullptr;

    ctx->java_raw_in = java_in;
    ctx->java_raw_out = java_out;
    ctx->element_count = count;
    ctx->element_size = elem_size;
    atomic_store_explicit(&ctx->status, 0, memory_order_relaxed);

    // Tính toán con trỏ căn chỉnh 64-byte cho P-Core
    ctx->pcore_aligned_in = ecore_align_pointer(java_in, CACHE_LINE);
    if (java_out != nullptr) {
        ctx->pcore_aligned_out = ecore_align_pointer(java_out, CACHE_LINE);
    } else {
        ctx->pcore_aligned_out = nullptr;
    }

    return ctx;
}

void ecore_ffi_destroy_context(ECoreFFIContext* ctx) {
    if (ctx != nullptr) {
        free(ctx);
    }
}

// =============================================================================
// WORKER CALLBACKS DÀNH RIÊNG CHO E-CORE
// =============================================================================

// Callback 1: E-Core chạy trước DAG để kiểm tra/chuẩn bị dữ liệu cho P-Core
static void* ecore_prepare_worker_func(TaskHandle* self, void* arg) {
    ECoreFFIContext* ctx = (ECoreFFIContext*)arg;
    
    // Đánh dấu trạng thái đang chuyển giao dữ liệu sang P-Core
    atomic_store_explicit(&ctx->status, 1, memory_order_release);

    // Nếu con trỏ Java truyền sang chưa được căn chỉnh 64-byte, E-Core tiến hành nhích bộ nhớ
    // hoặc thực hiện sắp xếp lại cấu trúc dữ liệu (Data Restructuring - Structure of Arrays)
    // trước khi giao cho P-Core xả tính toán.

    return ctx;
}

// Callback 2: E-Core chạy sau khi P-Core hoàn thành DAG để trả dữ liệu cho Java FFI
static void* ecore_finish_worker_func(TaskHandle* self, void* arg) {
    ECoreFFIContext* ctx = (ECoreFFIContext*)arg;

    // Đánh dấu toàn bộ pipeline FFI đã kết thúc
    atomic_store_explicit(&ctx->status, 2, memory_order_release);

    // Thực hiện bất kỳ tác vụ post-processing nào (ví dụ: kích hoạt Java Native Callback)
    return ctx;
}

// =============================================================================
// TASK CREATION HELPERS
// =============================================================================

TaskHandle* ecore_create_prepare_task(Thrd* pool, ECoreFFIContext* ctx, TaskPriority prio) {
    // Luôn ép target_core là CORE_TYPE_ECORE
    return pool_create_task_ex(pool, ecore_prepare_worker_func, ctx, prio, CORE_TYPE_ECORE);[cite: 1]
}

TaskHandle* ecore_create_finish_task(Thrd* pool, ECoreFFIContext* ctx, TaskPriority prio) {
    // Luôn ép target_core là CORE_TYPE_ECORE[cite: 1]
    return pool_create_task_ex(pool, ecore_finish_worker_func, ctx, prio, CORE_TYPE_ECORE);[cite: 1]
}