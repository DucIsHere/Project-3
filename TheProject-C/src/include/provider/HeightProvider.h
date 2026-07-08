#ifndef HEIGHT_PROVIDER_H
#define HEIGHT_PROVIDER_H

#include <stdint.h>

#include "include/util/FastRandom.h"

typedef struct 
{
     int32_t min;
     int32_t variation_min;
     int32_t variation_range;
} HeightProvider;

static inline HeightProvider lch(int32_t min, int32_t variation_min, int32_t variation_range)
{
     HeightProvider h;
     h.min = min;
     h.variation_min = variation_min;
     h.variation_range = variation_range;
}

// --- LOGIC SAMPLING METHOD ---
// Thay thế hoàn toàn sample(RandomSource random, WorldGenerationContext ctx) của Java
// Sử dụng hàm fr_next_int_bound tối ưu toán bit lũy thừa 2 của chính ông!
static inline int32_t lch_sample(const HeightProvider* h, FastRandom* rng) {
    // Thuật toán gốc: min + nextInt(variationMin + nextInt(variationRange))
    int32_t inner_rand = fr_next_int_bound(rng, h->variation_range);
    int32_t outer_bound = h->variation_min + inner_rand;
    
    return h->min + fr_next_int_bound(rng, outer_bound);
}

#endif
