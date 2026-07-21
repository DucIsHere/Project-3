#ifndef STAMPED_LONG_MAP_H
#define STAMPED_LONG_MAP_H

#include <stdbool.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>

#include "LongMap.h"

typedef struct StampedLongMapC
{
     LongMap* internal_map;
     _Atomic uint64_t stamp_counter;
};

StampedLongMapC* stamped_long_map_create(uint32_t capacity);
void stamped_long_map_destroy(StampedLongMapC* smap);

uint32_t stamped_long_map_size(StampedLongMapC* smap);
void stamped_long_map_clear(StampedLongMapC* smap);

void stamped_long_map_put(StampedLongMapC* smap, uint64_t key, uint64_t value);
uint64_t stamped_long_map_get(StampedLongMapC* smap, uint64_t key);

uint64_t stamped_long_map_remove(StampedLongMapC* smap, uint64_t key);
void stamped_long_map_remove_consumer(StampedLongMapC* smap, uint64_t key, ConsumerFunc consumer);
uint32_t stamped_long_map_remove_if(StampedLongMapC* smap, PredicateFunc predicate);

uint64_t stamped_long_map_compute_if_absent(StampedLongMapC* smap, uint64_t key, ComputeFunc computer);

#endif
