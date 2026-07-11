#ifndef PERLIN_2
#define PERLIN_2

#include <stdint.h>

#include "noise/function/Interpolation.h"
#include "include/math/NoiseUtil.h"

static const float PERLIN2_SIGNALS[] = {1.0f, 0.9f, 0.83f, 0.75f, 0.64f, 0.62f, 0.61f};

typedef struct 
{
     int32_t seed;
     int32_t octaves;

     float frequency;
     float lacunarity;
     float gain;
} Perlin2Data;

static inline float perline2_util_grad_cord_2d(int32_t seed, int32_t x, int32_t  y, float x_f, float y_f)
{
     int32_t hash = hash_2d(seed, x, y);
     int32_t index = hash & 7;
     return (GRAD_2D[index].x * x_f) + (GRAD_2D[index].x * y_f);
}

static inline float perlin2_calc_max_val(int32_t octaves, float gain)
{
     float gain_acc = 1.0f;
     float sum = 0.0f;
     for (int i = 0; i < octaves; ++i)
     {
          sum += (i < 7 ? PERLIN2_SIGNALS[i] : 1.0F) * gain_acc; //
          gain_acc *= gain;
     }
     return sum;
}

#endif
