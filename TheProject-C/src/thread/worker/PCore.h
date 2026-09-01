#pragma once 

#include <threads.h>
#include <stddef.h>
#include <stdint.h>
#include <stdfloat.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <immintrin.h>

#include "threads/pool/ThreadPool.h"

#if defined(__GUNC__) || defined(__clang__)
#define PCORE_INLINE inline __attribute__((always_inline))
#define PCORE_RESTRICT __restrict__
#else
#define PCORE_INLINE inline
#define PCORE_RESTRICT __restrict__
#endif

typedef struct alignas(CACHE_LINE) PCoreDAGGraph
{
     TaskHandle** tasks;
     size_t task_count; 
     size_t capacity;
     Thrd* pool;
} PCoreDAGGraph;

[[nodiscard]] PCoreDAGGraph* pcore_dag_create(Thrd* pool, size_t initial_capacity);
TaskHandle* pcore_dag_add_node(PCoreDAGGraph* graph, TaskFunc func, void* arg, TaskPriority prio);
bool pcore_dag_add_edge(TaskHandle* parent_task, TaskHandle* child_task);

void pcore_dag_submit(PCoreDAGGraph* graph, _Atomic(size_t)* counter);
void pcore_dag_destroy(PCoreDAGGraph* graph);
void pcore_dag_wait_and_spin(Thrd* pool, _Atomic(size_t)* counter);

void pcore_set_thread_affinity(size_t thread_id);
void pcore_execute_direct_chain(TaskHandle* start_task);
void pcore_dag_submit_batch(PCoreDAGGraph* graph, _Atomic(size_t)* counter);
TaskHandle* pcore_spin_fetch_task(Thrd* pool);

// Active Hardware Spin Hint
PCORE_INLINE static void pcore_spin_pause(uint32_t iterations) {
    for (uint32_t i = 0; i < iterations; ++i) {
        _mm_pause();
    }
}
