#pragma once 

#include <stddef.h>
#include <stdint.h>
#include <stdbits.h>
#include <stdbool.h>

typedef struct Disposable
{
     void (*dispose)(void* ctx);
     void* ctx;
} Disposable;

[[gnu::always_inline]] static inline void disposable_run(Disposable* d)
{
     if (d && d->dispose)
     {
          d->dispose(d->ctx);
     }
}

[[gnu::always_inline]] static inline bool disposable_mask_all_cleared(uint64_t active_mask)
{
     return stdc_count_ones(active_mask) == 0;
}

[[gnu::always_inline]] static inline void _auto_disposable_guard(Disposable** d)
{
     if (d && *d)
     {
          disposable_run(*d);
     }
}

#define SCOPED_DISPOSABLE(var_name, disposable_ptr) \
     Disposable* var_name [[gnu::cleanup(_auto_dispose_guard)]] = (disposable_ptr)