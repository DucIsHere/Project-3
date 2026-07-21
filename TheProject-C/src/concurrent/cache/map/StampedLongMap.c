#include "StampedLongMap.h"

StampedLongMapC* stamped_long_map_create(uint32_t capacity) {
    StampedLongMapC* smap = (StampedLongMapC*)malloc(sizeof(StampedLongMapC));
    smap->internal_map = longmap_create(capacity);
    atomic_store_explicit(&smap->stamp_counter, 1, memory_order_relaxed);
    return smap;
}

void stamped_long_map_destroy(StampedLongMapC* smap) {
    if (!smap) return;
    if (smap->internal_map) {
        longmap_destroy(smap->internal_map);
    }
    free(smap);
}

uint32_t stamped_long_map_size(StampedLongMapC* smap) {
    return longmap_size(smap->internal_map);
}

void stamped_long_map_clear(StampedLongMapC* smap) {
    longmap_clear(smap->internal_map);
    atomic_fetch_add_explicit(&smap->stamp_counter, 1, memory_order_relaxed);
}

void stamped_long_map_put(StampedLongMapC* smap, uint64_t key, uint64_t value) {
    longmap_put(smap->internal_map, key, value);
    atomic_fetch_add_explicit(&smap->stamp_counter, 1, memory_order_relaxed);
}

uint64_t stamped_long_map_get(StampedLongMapC* smap, uint64_t key) {
    return longmap_get(smap->internal_map, key);
}

uint64_t stamped_long_map_remove(StampedLongMapC* smap, uint64_t key) {
    uint64_t res = longmap_remove(smap->internal_map, key);
    if (res != 0) {
        atomic_fetch_add_explicit(&smap->stamp_counter, 1, memory_order_relaxed);
    }
    return res;
}

void stamped_long_map_remove_consumer(StampedLongMapC* smap, uint64_t key, ConsumerFunc consumer) {
    longmap_remove_consumer(smap->internal_map, key, consumer);
}

uint32_t stamped_long_map_remove_if(StampedLongMapC* smap, PredicateFunc predicate) {
    return longmap_remove_if(smap->internal_map, predicate);
}

uint64_t stamped_long_map_compute_if_absent(StampedLongMapC* smap, uint64_t key, ComputeFunc computer) {
    return longmap_compute_if_absent(smap->internal_map, key, computer);
}