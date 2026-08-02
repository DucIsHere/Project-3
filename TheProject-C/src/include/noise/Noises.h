#ifndef NOISES_H
#define NOISES_H

#include <stdint.h>
#include <stdlib.h>

#include "noise/domain/Domain.h"
#include "noise/domain/Domains.h"
#include "noise/function/CellFunction.h"
#include "noise/function/Interpolation.h"
#include "noise/function/EdgeFunction.h"
#include "noise/function/CellFunction.h"
#include "include/noise/Noise.h"
#include "noise/module/Abs"
#include "noise/module/ShiftSeed.h"
#include "noise/module/Sin.h"
#include "noise/module/White.h"
#include "noise/module/Perlin.h"
#include "noise/module/Perlin2.h"

static inline Noise* constant(float value)
{
    return Constant(value);
}

static inline Noise* sin(int32_t seed, float frequency, Noise* alpha)
{
    Noise* sin_node = sin_create(frequency, alpha);
    return shift_seed_create(sin_node, seed);
}

static inline Noise* white(int32_t seed, int32_t scale)
{
    Noise* white_node = white_make(1.0f / scale);
    return shift_seed_create(white_node, seed);
}

static inline Noise* perlin(int32_t seed, int32_t scale, int32_t octave)
{
    return noises_perlin_create(seed, scale, octave, 2.0f);
}

static inline Noise* perlin(int32_t seed, int32_t scale, int32_t octave, int32_t lacunarity)
{
    return noises_perlin_create(seed, scale, octave, lacunarity, 0.5f);
}

static inline Noise* perlin(int32_t seed, int32_t scale, int32_t octave, int32_t lacunarity, float gain)
{
    return Perlin(seed, 1.0f / scale, octave, lacunarity, gain, INTERPOLATION_CURVE3);
}

static inline Noise* perlin2(int32_t seed, int32_t scale, int32_t octave)
{
    return noise_perlin2_create(seed, scale, octave, 2.0f);
}

static inline Noise* perlin2(int32_t seed, int32_t scale, int32_t octave, int32_t lacunarity)
{
    return noise_perlin2_create(seed, scale, octave, lacunarity, 0.5f);
}

static inline Nosie* perlin2(int32_t seed, int32_t scale, int32_t octave, int32_t lacunarity, float gain)
{
    return noise_perlin2_create(seed, 1.0f / scale, octave, lacunarity, gain, INTERPOLATION_CURVE3);
}

#endif
