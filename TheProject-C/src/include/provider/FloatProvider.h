#ifndef FLOAT_PROVIDER_H
#define FLOAT_PROVIDER_H

#include <stdint.h>

#include "include/util/FastRandom.h"

static inline float fp_legacy_canyon_y_scale_sample(FastRandom* rng)
{
     return (fr_next_float(rng) - 0.5f) * 2.0f / 8.0f;
}

static inline float fp_legacy_canyon_y_scale_get_min_value(void)
{
     return -1.0f;
}

static inline float fp_legacy_canyon_y_scale_get_max_value(void)
{
     return 1.0f;
}

#endif
