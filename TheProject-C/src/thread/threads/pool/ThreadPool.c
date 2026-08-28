#include "ThreadPool.h"

#include <stdint.h>
#include <stdlib.h>
#include <threads.h>
#include <unistd.h>
#include <sched.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <linux/futex.h>

thread_local size_t g_workder_thread_id = 0;

static long sys_futex(void* uaddr, int futex_op, uint32_t val, const struct timespec* time_out, void* uaddr2, uint32_t val3)
{
     return syscall(SYS_futex, uaddr, futex_op, val, time_out, uaddr2, val3);
}

static void futex_wait_private(_Atomic uint32_t* futex_word, uint32_t expected)
{
     sys_futex((void*)futex_word, FUTEX_WAIT_PRIVATE, expected, NULL, NULL, 0);
}

static void futex_wake_private(_Atomic uint32_t* futex_word, int count)
{
     sys_futex((void*)futex_word, FUTEX_WAKE_PRIVATE, count, NULL, NULL, 0);
}

static void pool_bind_thread_affinity(size_t thread_id)
{
     cpu_set_t cpuset;
     CPUZERO(&cpuset);
     CPU_SET(thread_id, &cpuset);
     pthread_setaffinity_np(pthread_self(), sizeof(&cpuset), &cpuset);
}

static void init_info(Thrd* pool)
{
     for (size_t i = 0; i < TOTAL_HARDWARE_THREAD; i++)
     {
          pool->info[i].thread_id = i;
          if (i < 16)
          {
               pool->info[i].core_id = (i / 2) + i;
               pool->info[i].core_type = CORE_TYPE_PCORE;
               pool->info[i].is_smt_thread = (i % 2 != 0);
          } else {
               pool->info[i].core_id = (i - 16) + 9;
               pool->info[i].core_type = CORE_TYPE_ECORE;
               pool->info[i].is_smt_thread = false;
          }
     }
}

static inline TaskHandle* arena_lock_task(Thrd* pool)
{
     size_t idx = atomic_fetch_add_explicit(pool->memory_arena.arena_idx, 1, memory_order_relaxed);
     idx = idx % (TASK_QUEUE_CAPACITY * 2);
     TaskHandle* task = &pool->memory_arena.pool_arena[idx];
     
}
