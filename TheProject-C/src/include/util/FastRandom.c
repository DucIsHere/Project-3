#include "include/util/FastRandom.h" 

#define EXPORT __attribute__((visibility("default")))

// 1. Cổng khởi tạo
EXPORT void FastRandom_Init(FastRandom* rng, uint64_t seed) {
    fr_init(rng, seed);
}

EXPORT void FastRandom_InitFull(FastRandom* rng, uint64_t seed, uint64_t gamma) {
    fr_init_full(rng, seed, gamma);
}

// 2. Cổng sinh số ngẫu nhiên
EXPORT int32_t FastRandom_NextInt(FastRandom* rng) {
    return fr_next_int(rng);
}

EXPORT int32_t FastRandom_NextIntBound(FastRandom* rng, int32_t bound) {
    return fr_next_int_bound(rng, bound);
}

EXPORT float FastRandom_NextFloat(FastRandom* rng) {
    return fr_next_float(rng);
}

EXPORT bool FastRandom_NextBoolean(FastRandom* rng) {
    return fr_next_boolean(rng);
}