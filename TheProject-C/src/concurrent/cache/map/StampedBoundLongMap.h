#ifndef STAMPED_BOUND_LONG_MAP_H
#define STAMPED_BOUND_LONG_MAP_H

#include "LongMap.h"

typedef struct BoundMapNode {
    uint64_t key;
    uint64_t value;
    struct BoundMapNode* prev;
    struct BoundMapNode* next;
} BoundMapNode;

typedef struct {
    LongMap* internal_map;       // Slot lookup
    uint32_t capacity;           // Dung lượng tối đa
    _Atomic uint64_t sequence;   // Stamp versioning
    
    // Con trỏ đầu/cuối của Linked List để phục vụ removeFirst (LRU)
    BoundMapNode* head;
    BoundMapNode* tail;
} StampedBoundLongMapC;

// Prototypes C thuần
StampedBoundLongMapC* stamped_bound_map_create(uint32_t capacity);
void stamped_bound_map_destroy(StampedBoundLongMapC* smap);

uint32_t stamped_bound_map_size(StampedBoundLongMapC* smap);
void stamped_bound_map_clear(StampedBoundLongMapC* smap);

void stamped_bound_map_put(StampedBoundLongMapC* smap, uint64_t key, uint64_t value);
uint64_t stamped_bound_map_get(StampedBoundLongMapC* smap, uint64_t key);

uint64_t stamped_bound_map_remove(StampedBoundLongMapC* smap, uint64_t key);
void stamped_bound_map_remove_consumer(StampedBoundLongMapC* smap, uint64_t key, ConsumerFunc consumer);
uint32_t stamped_bound_map_remove_if(StampedBoundLongMapC* smap, PredicateFunc predicate);

uint64_t stamped_bound_map_compute_if_absent(StampedBoundLongMapC* smap, uint64_t key, ComputeFunc func);

#endif // STAMPED_BOUND_LONG_MAP_H