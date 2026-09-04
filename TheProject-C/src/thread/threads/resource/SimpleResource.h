#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdbit.h>
#include "Resource.h"

typedef struct alignas(64) SimpleResource {
    Resource* target_resource;
    void (*closer)(void* handle);
    bool is_open;
} SimpleResource;

[[gnu::always_inline]] static inline SimpleResource simple_resource_make(Resource* res, void (*closer)(void*)) {
    return (SimpleResource){
        .target_resource = res,
        .closer = closer,
        .is_open = false
    };
}

[[gnu::always_inline]] static inline void* simple_resource_get(SimpleResource* sres) {
    if (!sres || !sres->target_resource) return NULL;
    sres->is_open = true;
    return resource_get(sres->target_resource);
}

[[gnu::always_inline]] static inline void simple_resource_close(SimpleResource* sres) {
    if (sres && sres->is_open) {
        sres->is_open = false;
        
        if (sres->target_resource) {
            atomic_store_explicit(&sres->target_resource->state, 0, memory_order_release);
        }
        
        if (sres->closer && sres->target_resource) {
            sres->closer(sres->target_resource->handle);
        }
    }
}

[[gnu::always_inline]] static inline void _auto_close_simple_resource(SimpleResource** sres) {
    if (sres && *sres) {
        simple_resource_close(*sres);
    }
}

#define SCOPED_SIMPLE_RESOURCE(var_name, sres_ptr) \
    SimpleResource* var_name [[gnu::cleanup(_auto_close_simple_resource)]] = (sres_ptr)