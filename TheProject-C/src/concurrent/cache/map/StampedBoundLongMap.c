#include "StampedBoundLongMap.h"

// Khai báo helper nội bộ quản lý node Linked List
static inline void remove_node(StampedBoundLongMapC* smap, BoundMapNode* node) {
    if (node->prev) node->prev->next = node->next;
    else smap->head = node->next;

    if (node->next) node->next->prev = node->prev;
    else smap->tail = node->prev;
}

static inline void append_node(StampedBoundLongMapC* smap, BoundMapNode* node) {
    node->next = NULL;
    node->prev = smap->tail;

    if (smap->tail) {
        smap->tail->next = node;
    } else {
        smap->head = node;
    }
    smap->tail = node;
}

StampedBoundLongMapC* stamped_bound_map_create(uint32_t capacity) {
    StampedBoundLongMapC* smap = (StampedBoundLongMapC*)malloc(sizeof(StampedBoundLongMapC));
    smap->internal_map = longmap_create(capacity);
    smap->capacity = capacity;
    smap->head = NULL;
    smap->tail = NULL;
    atomic_store_explicit(&smap->sequence, 0, memory_order_relaxed);
    return smap;
}

void stamped_bound_map_destroy(StampedBoundLongMapC* smap) {
    if (!smap) return;
    stamped_bound_map_clear(smap);
    if (smap->internal_map) {
        longmap_destroy(smap->internal_map);
    }
    free(smap);
}

uint32_t stamped_bound_map_size(StampedBoundLongMapC* smap) {
    return longmap_size(smap->internal_map);
}

void stamped_bound_map_clear(StampedBoundLongMapC* smap) {
    atomic_fetch_add_explicit(&smap->sequence, 1, memory_order_release);
    
    BoundMapNode* curr = smap->head;
    while (curr) {
        BoundMapNode* next = curr->next;
        free(curr);
        curr = next;
    }
    smap->head = NULL;
    smap->tail = NULL;
    longmap_clear(smap->internal_map);

    atomic_fetch_add_explicit(&smap->sequence, 1, memory_order_release);
}

void stamped_bound_map_put(StampedBoundLongMapC* smap, uint64_t key, uint64_t value) {
    atomic_fetch_add_explicit(&smap->sequence, 1, memory_order_release);

    uint64_t old_node_ptr = longmap_get(smap->internal_map, key);
    if (old_node_ptr != 0) {
        BoundMapNode* node = (BoundMapNode*)old_node_ptr;
        node->value = value;
        remove_node(smap, node);
        append_node(smap, node);
    } else {
        // Tái hiện logic removeFirst() nếu đụng trần Capacity
        if (longmap_size(smap->internal_map) >= smap->capacity && smap->head != NULL) {
            BoundMapNode* first = smap->head;
            longmap_remove(smap->internal_map, first->key);
            remove_node(smap, first);
            free(first);
        }

        BoundMapNode* new_node = (BoundMapNode*)malloc(sizeof(BoundMapNode));
        new_node->key = key;
        new_node->value = value;
        append_node(smap, new_node);
        longmap_put(smap->internal_map, key, (uint64_t)new_node);
    }

    atomic_fetch_add_explicit(&smap->sequence, 1, memory_order_release);
}

uint64_t stamped_bound_map_get(StampedBoundLongMapC* smap, uint64_t key) {
    uint64_t stamp_before, stamp_after, node_ptr;

    do {
        stamp_before = atomic_load_explicit(&smap->sequence, memory_order_acquire);
        while (stamp_before & 1) {
            cpu_relax();
            stamp_before = atomic_load_explicit(&smap->sequence, memory_order_acquire);
        }

        node_ptr = longmap_get(smap->internal_map, key);
        stamp_after = atomic_load_explicit(&smap->sequence, memory_order_acquire);
    } while (stamp_before != stamp_after);

    if (node_ptr == 0) return 0;
    return ((BoundMapNode*)node_ptr)->value;
}

uint64_t stamped_bound_map_remove(StampedBoundLongMapC* smap, uint64_t key) {
    atomic_fetch_add_explicit(&smap->sequence, 1, memory_order_release);

    uint64_t node_ptr = longmap_remove(smap->internal_map, key);
    uint64_t val = 0;

    if (node_ptr != 0) {
        BoundMapNode* node = (BoundMapNode*)node_ptr;
        val = node->value;
        remove_node(smap, node);
        free(node);
    }

    atomic_fetch_add_explicit(&smap->sequence, 1, memory_order_release);
    return val;
}

void stamped_bound_map_remove_consumer(StampedBoundLongMapC* smap, uint64_t key, ConsumerFunc consumer) {
    uint64_t val = stamped_bound_map_remove(smap, key);
    if (val != 0 && consumer != NULL) {
        consumer(val);
    }
}

uint32_t stamped_bound_map_remove_if(StampedBoundLongMapC* smap, PredicateFunc predicate) {
    atomic_fetch_add_explicit(&smap->sequence, 1, memory_order_release);

    uint32_t removed_count = 0;
    BoundMapNode* curr = smap->head;

    while (curr) {
        BoundMapNode* next = curr->next;
        if (predicate(curr->value)) {
            longmap_remove(smap->internal_map, curr->key);
            remove_node(smap, curr);
            free(curr);
            removed_count++;
        }
        curr = next;
    }

    atomic_fetch_add_explicit(&smap->sequence, 1, memory_order_release);
    return removed_count;
}

uint64_t stamped_bound_map_compute_if_absent(StampedBoundLongMapC* smap, uint64_t key, ComputeFunc func) {
    uint64_t val = stamped_bound_map_get(smap, key);
    if (val != 0) return val;

    atomic_fetch_add_explicit(&smap->sequence, 1, memory_order_release);

    // Double check sau khi lấy Write Stamp
    uint64_t node_ptr = longmap_get(smap->internal_map, key);
    if (node_ptr != 0) {
        val = ((BoundMapNode*)node_ptr)->value;
    } else if (func != NULL) {
        val = func(key);
        if (val != 0) {
            // Tự động removeFirst nếu vượt quá capacity
            if (longmap_size(smap->internal_map) >= smap->capacity && smap->head != NULL) {
                BoundMapNode* first = smap->head;
                longmap_remove(smap->internal_map, first->key);
                remove_node(smap, first);
                free(first);
            }

            BoundMapNode* new_node = (BoundMapNode*)malloc(sizeof(BoundMapNode));
            new_node->key = key;
            new_node->value = val;
            append_node(smap, new_node);
            longmap_put(smap->internal_map, key, (uint64_t)new_node);
        }
    }

    atomic_fetch_add_explicit(&smap->sequence, 1, memory_order_release);
    return val;
}