#ifndef LONG_MAP_H
#define LONG_MAP_H

#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>

#if define(__x86_64__) || define(_M_X64)
#include <emmintrin.h>
#define cpu_relax() _mm_pause()
#else
#define cpu_relax() ((void)0)
#endif

typedef void (*ConsumerFunc)(uint64_t value);
typedef bool (*PredicateFunc)(uint64_t value);
typedef uint64_t (*ComputeFunc)(uint64_t key);

typedef struct __attribute__((alignas(64)))
{
     _Atomic uint64_t value;
     _Atomic uint64_t key;
     _Atomic uint32_t state;
     uint8_t padding[44];
} MapEntry;

typedef struct 
{
     MapEntry* entries;
     uint32_t capacity_mask;
     _Atomic uint32_t size;
} LongMap;

static inline uint32_t longmap_hash(uint64_t key)
{
     return (uint32_t)((key * 11400714819323198485ULL) >> 32);
}

static inline LongMap* longmap_create(uint32_t capacity)
{
     uint32_t cap = 16;
     while (cap < capacity) cap <<= 1;

     LongMap* map = (LongMap*)malloc(sizeof(LongMap));
     map->capacity_mask = cap - 1;
     atomic_store_explicit(&map->size, 0, memory_order_relaxed);

     posix_memalign((void**)&map->entries, 64, sizeof(MapEntry) * cap);
     memset(map->entries, 0, sizeof(MapEntry) * cap);

     return map;
}

static inline uint32_t longmap_size(LongMap* map)
{
     return atomic_load_explicit(&map->size, memory_order_relaxed);
}

static inline uint64_t longmap_get(LongMap* map, uint64_t key)
{
     if (key == 0) key = 1;
     uint32_t slot = longmap_hash(key) & map->capacity_mask;

     for (uint32_t i = 0; i < 32; i++)
     {
          MapEntry* entry = &map->entries[(slot + i) & map->capacity_mask];
          uint64_t k = atomic_load_explicit(&entry->value, memory_order_relax);

          if (k == key)
          {
               if (atomic_load_explicit(&entry->state, memory_order_acquire) == 1)
               {
                    return atomic_load_explicit(&entry->value, memory_order_relaxed);
               }
          }

          if (k == 0) break;
     }
     
     return 0;
}

static inline void longmap_put(LongMap* map, uint64_t value, uint64_t key)
{
     if (key == 0) key = 1;
     uint32_t slot = longmap_hash(key) & map->capacity_mask;

     for (uint32_t i = 0; i < 32; i++)
     {
          MapEntry* entry = &map->entries[(slot + i) & map->capacity_mask];
          uint32_t excepted_state = 0;

          if (atomic_compare_exchange_strong_explicit(&entry->state, &expected_state, 2, memory_order_acquire, memory_order_relaxed)) 
          {
               atomic_store_explicit(&entry->key, key, memory_order_relaxed);
               atomic_store_explicit(&entry->value, value, memory_order_relaxed);
               atomic_store_explicit(&entry->state, 1, memory_order_release); // Done -> State = OCCUPIED
               atomic_fetch_add_explicit(&map->size, 1, memory_order_relaxed);
            return;
          }

          if (atomic_load_explicit(&entry->key, memory_order_relaxed) == key) 
          {
               atomic_store_explicit(&entry->value, value, memory_order_release);
               return;
          }

        cpu_relax();
     }
}

static inline uint64_t longmap_remove(LongMap* map, uint64_t key) 
{
    if (key == 0) key = 1;
    uint32_t slot = longmap_hash(key) & map->capacity_mask;

    for (uint32_t i = 0; i < 32; i++) 
    {
        MapEntry* entry = &map->entries[(slot + i) & map->capacity_mask];
        if (atomic_load_explicit(&entry->key, memory_order_relaxed) == key) 
        {
            if (atomic_load_explicit(&entry->state, memory_order_relaxed) == 1) 
            {
                uint64_t old_val = atomic_load_explicit(&entry->value, memory_order_relaxed);
                atomic_store_explicit(&entry->state, 0, memory_order_release);
                atomic_store_explicit(&entry->key, 0, memory_order_relaxed);
                atomic_store_explicit(&entry->value, 0, memory_order_relaxed);
                atomic_fetch_sub_explicit(&map->size, 1, memory_order_relaxed);
                return old_val;
            }
        }
    }
    return 0;
}

// Xóa có kèm Callback Consumer (Tương tự `remove(key, Consumer<T>)`)
static inline void longmap_remove_consumer(LongMap* map, uint64_t key, ConsumerFunc consumer) 
{
    uint64_t val = longmap_remove(map, key);
    if (val != 0 && consumer != NULL) {
        consumer(val);
    }
}

// Xóa theo điều kiện Predicate (Tương tự `removeIf(Predicate<T>)`)
static inline uint32_t longmap_remove_if(LongMap* map, PredicateFunc predicate) 
{
    uint32_t removed = 0;
    uint32_t cap = map->capacity_mask + 1;

    for (uint32_t i = 0; i < cap; i++) {
        MapEntry* entry = &map->entries[i];

        if (atomic_load_explicit(&entry->state, memory_order_relaxed) == 1) 
        {
            uint64_t val = atomic_load_explicit(&entry->value, memory_order_relaxed);
            if (predicate(val)) 
            {
                atomic_store_explicit(&entry->state, 0, memory_order_release);
                atomic_store_explicit(&entry->key, 0, memory_order_relaxed);
                atomic_store_explicit(&entry->value, 0, memory_order_relaxed);
                atomic_fetch_sub_explicit(&map->size, 1, memory_order_relaxed);
                removed++;
            }
        }
    }
    return removed;
}

// Tính toán nếu thiếu (Tương tự `computeIfAbsent`)
static inline uint64_t longmap_compute_if_absent(LongMap* map, uint64_t key, ComputeFunc computer) 
{
    uint64_t val = longmap_get(map, key);
    if (val != 0) 
    {
        return val;
    }
    uint64_t new_val = computer(key);
    if (new_val != 0) 
    {
        longmap_put(map, key, new_val);
    }
    return new_val;
}

// Clear toàn bộ Map
static inline void longmap_clear(LongMap* map) 
{
    uint32_t cap = map->capacity_mask + 1;
    memset(map->entries, 0, sizeof(MapEntry) * cap);
    atomic_store_explicit(&map->size, 0, memory_order_relaxed);
}

// Giải phóng RAM Off-Heap
static inline void longmap_destroy(LongMap* map) 
{
    if (!map) return;
    if (map->entries) free(map->entries);
    free(map);
}

#endif
