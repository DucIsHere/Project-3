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
     {}
}

#endif
