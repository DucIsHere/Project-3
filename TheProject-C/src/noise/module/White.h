#ifndef WHITE_H
#define WHITE_H

#include <stdint.h>

#include "include/noise/Noise.h"
#include "include/math/NoiseUtil.h"

typedef struct 
{
    float frequency
} WhiteTypez;

static inline Noise white_compute(float x, float z, int32_t seed)
{
    x *= w->frequency;
    z *= w->frequency;

    float value = sample(x, z, seed);

    return abs(value);
}

static inline float sample(float x, float z, int32_t seed)
{
    int32_t xi = round(x);
    int32_t zi = round(z);
    return valCoord2D(seed, xi, zi);
}

static inline float sample(float x, float z, int32_t seed, int32_t offset)
{
    return sample(x, z, hash(seed, offset));
}

static inline Noise white_make(WhiteTypez* w, float frequency)
{
    Noise n;
    n.type = NOISE_TYPEDEG_WHITE;
    n.node_ref = NULL;

    n.min_value = 0.0f;
    n.max_value = 1.0f;

    WhiteTypez* w = (WhiteTypez*).n.data.custom_data;
    w->frequency = frequency;

    return n;
}

#endif
