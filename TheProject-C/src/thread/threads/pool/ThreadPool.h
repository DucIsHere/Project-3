#ifndef THREAD_POOL
#define THREAD_POOL
#define CACHE_LINE 64

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <threads.h>
#include <stdatomic.h>

constexpr size_t CACHE_LINE = 64;
constexpr size_t TASK_QUEUE_CAPACITY = 8192;
constexpr size_t TOTAL_HARDWARE_THREADS = 24;
constexpr size_t TOTAL_HARDWARE_CORES = 16;

typedef enum 
{
     THREAD_0 = 0,
     THREAD_1 = 1,

     THREAD_2 = 2,
     THREAD_3 = 3,

     THREAD_4 = 4,
     THREAD_5 = 5,

     THREAD_6 = 6,
     THREAD_7 = 7,

     THREAD_8 = 8,
     THREAD_9 = 9,

     THREAD_10 = 10,
     THREAD_11 = 11,

     THREAD_12 = 12,
     THREAD_13 = 13,

     THREAD_14 = 14,
     THREAD_15 = 15,

     THREAD_16 = 16,
     THREAD_17 = 17,
     THREAD_18 = 18,
     THREAD_19 = 19, 
     THREAD_20 = 20,
     THREAD_21 = 21, 
     THREAD_22 = 22,
     THREAD_23 = 23
} Threads;

typedef enum 
{
     CORE_TYPE_PCORE = 0,
     CORE_TYPE_ECORE = 1
} CoreType;

typedef enum 
{
     TASK_PRIO_CRITICAL = 0,
     TASK_PRIO_HIGH = 1,
     TASK_PRIO_NORMAL = 2,
     TASK_PRIO_LOW = 3,
     TASK_PRIO_COUNT = 4
} TaskPriority;

typedef enum
{
     TASK_STATE_FREE = 0,
     TASK_STATE_WAITING_DEP,
     TASK_STATE_READY,
     TASK_STATE_EXCUTING,
     TASK_STATE_FINISHED
} TaskState;

typedef struct 
{
     size_t thread_id;
     size_t core_id;
     CoreType core_type;
     bool is_smt_thread;
} ThreadInfo;

typedef struct DependencyNode
{
     TaskHandle* task;
     struct DependencyNode* next;
} DependencyNode;

typedef void* (*TaskFunc)(TaskHandle* self, void* arg);

struct TaskHandle
{
     TaskFunc func;
     void* arg;
     _Atomic(size_t)* counter;
     _Atomic(void*)* feature_result;

     _Atomic(int32_t) dependency_count;
     _Atomic(DependencyNode*) dependents_head;
     _Atomic(TaskState) state;

     TaskHandle* parent;
     _Atomic(size_t) active;

     TaskPriority priority;
     CoreType target_core;
     Thrd* pool;
};

typedef struct 
{
     void (*function)(void* args);
     void* args;
     _Atomic(size_t)* counter;
} Task;

typedef struct
{
     _Atomic size_t squence;
     TaskHandle* task;
} QueueCell;

typedef struct alignas(CACHE_LINE)
{
     QueueCell ring[TASK_QUEUE_CAPACITY];
     _Atomic size_t head;
     _Atomic size_t tail;
} MPMCQueue;

typedef struct 
{
     _Atomic(size_t) in_barrier;
     _Atomic(size_t) out_barrier;
     size_t total_thread;
} Barrier;

typedef struct Thrd Thrd;
typedef struct TaskHandle TaskHandle;

typedef struct 
{
     size_t start_idx;
     size_t end_idx;
     size_t thread_id;
     void* user_data;
     Thrd* pool;
} ParallelRange;

typedef void (*ParallelFunc)(ParallelRange* range);

struct Thrd 
{
     thrd_t threads[TOTAL_HARDWARE_THREADS];
     ThreadInfo info[TOTAL_HARDWARE_THREADS];
     size_t thread_count;
     _Atomic bool shutdown;

     MPMCQueue p_queues[TASK_PRIO_COUNT];
     MPMCQueue e_queues[TASK_PRIO_COUNT];

     struct 
     {
          Task task[TASK_QUEUE_CAPACITY];
          _Atomic size_t head;
          _Atomic size_t tail;
          cnd_t signals;
          mtx_t lock;
     } queue;

     struct
     {
          ParallelFunc func;
          void* user_data;
          size_t total_elements;
          _Atomic(size_t) pending_workers;
          Barrier barrier;
          cnd_t dispath_signals;
          mtx_t dispath_lock;
          _Atomic bool pde_mode;
     } pde;

     cnd_t wake_signals;
     mtx_t wake_lock;

     struct alignas(CACHE_LINE)
     {
          TaskHandle pool_arena[TASK_QUEUE_CAPACITY * 2];
          _Atomic(size_t) arena_idx;
     } memory_arena;
};

[[nodiscard]] Thrd* pool_create(size_t thread_count);
Thrd* pool_make(void);

void pool_destroy(Thrd* pool);

void pool_bind_thread_affinity(size_t thread_id);

void pool_submit(Thrd* pool, void(*func)(void*), void* args, _Atomic(size_t)* counter);
void pool_wait(_Atomic(size_t)* counter);

[[nodicard]] TaskHandle* pool_create_task_ex(Thrd* pool, TaskFunc func, void* arg, TaskPriority prio, CoreType target);
void pool_add_dependency(TaskHandle* parent, TaskHandle* dependent);
void pool_submit_task(TaskHandle* task, _Atomic(size_t)* counter, _Atomic(size_t)* future_out);

void pool_wait_active(Thrd* pool, _Atomic(size_t)* counter);

void pool_fork_active(TaskHandle* parent_task, TaskFunc sub_func, void** arg_array, size_t subtask_count);

void pool_pde_parallel(Thrd* pool, size_t total_element, ParallelFunc func, void* user_data);
void pool_pde_barrier(ParallelRange* range);

#endif
