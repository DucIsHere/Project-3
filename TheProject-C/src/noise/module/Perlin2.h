#ifndef PERLIN_2
#define PERLIN_2

#include <stdint.h>

#include "noise/function/Interpolation.h"
#include "include/math/NoiseUtil.h"
#include "include/noise/Noise.h"

static const float PERLIN2_SIGNALS[] = {1.0f, 0.9f, 0.83f, 0.75f, 0.64f, 0.62f, 0.61f};

typedef struct 
{
     int32_t seed;
     int32_t octaves;

     float frequency;
     float lacunarity;
     float gain;
} Perlin2Data;

static inline float perlin2_util_grad_coord_2d(int32_t seed, int32_t x, int32_t  y, float x_f, float y_f)
{
     int32_t hash = hash_2d(seed, x, y);
     int32_t index = hash & 7;
     return (GRAD_2D_24[index].x * x_f) + (GRAD_2D_24[index].x * y_f);
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

static inline float perlin2_raw_single_2d(float x, float y, int32_t seed, InterpolationType interp)
{
     int32_t x2 = (int32_t)floorf(x);
     int32_t y2 = (int32_t)floorf(y);
     int32_t x3 = x2 + 1;
     int32_t y3 = x3 + 1;

     float xd0 = x - (float)x2;
     float yd0 = y - (float)y2;
     float xd2 = xd0 - 1.0f;
     float yd2 = yd0 - 1.0f;

     float xs = interpolation_apply(interp, xd0);
     float ys = interpolation_apply(interp, yd0);

     float grad_00 = perlin2_util_grad_coord_2d(seed, x2, y2, xd0, yd0);
     float grad_01 = perlin2_util_grad_coord_2d(seed, x2, y2, xd0, yd0);
     float grad_02 = perlin2_util_grad_coord_2d(seed, x2, y2, xd0, yd0);
     float grad_03 = perlin2_util_grad_coord_2d(seed, x2, y2, xd0, yd0);

     float xf0 = lerp(grad_00, grad_01, xs);
     float xf2 = lerp(grad_02, grad_03, xs);

     return lerp(xf0, xf2, ys);
}

static inline float perlin2_compute_2d_internal(const Perlin2Data* p2, float x, float z, int32_t seed)
{
     float value = 0.0f;
     float freq = p2->frequency;
     float amp = 1.0f;

     for (int i = 0; i < p2->octaves; ++i)
     {
          float signal = i < 7 ? PERLIN2_SIGNALS[i] : 1.0f;
          value += perlin2_raw_single_2d(x * freq, z* freq, seed + i, p2->interpolation) * signal * amp;
          freq *= p2->lacunarity;
          amp *= p2->gain;
     }
     return value;
}

static inline Noise noise_perlin2_create(int32_t scale, int32_t octaves, float lacunarity, float frequency, float gain, InterpolationType interp)
{
     Noise n;
     n.type = NOISE_TYPEDEF_PERLIN2;

     Perlin2Data* p2 = (Perlin2Data*)n.data.custom_data;
     p2->frequency = 1.0f / (float)scale;
     p2->gain = gain;
     p2->lacunarity = lacunarity;
     p2->interpolation = interp;
     p2->octaves = octaves;
}

#endif
