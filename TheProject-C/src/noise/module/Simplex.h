#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <stdint.h>
#include <math.h>

#include "include/noise/Noise.h"
#include "include/math/NoiseUtil.h"

static const float SIMPLEX_SIGNALS[] = {1.0f, 0.989f, 0.81f, 0.781f, 0.708f, 0.702f, 0.696f};

typedef struct
{
     float frequency;
     float lacunarity;
     float gain;
     
     int32_t octaves;
} SimplexData;

static inline float simplex_util_grad_coord_2d_24(int32_t seed, int32_t x, int32_t z, float x_f, float z_f)
{
     int32_t hash = hash_2d(seed, x, z);
     int32_t index = hash & 31;
     return (GRAD_2D_24[index].x * x_f) + (GRAD_2D_24[index].y * z_f);
}

static inline float cal_max_val(int32_t octaves, float gain) 
{
     float gain_acc = 1.0f;
     float sum = 0.0f;
     for (int32_t i = 0; i < octaves; ++i)
     {
          sum += (i < 7 ? SIMPLEX_SIGNALS[i] : 1.0f) * gain_acc;
          gain_acc *= gain;
     }
     return sum;
}

static inline float simplex_raw_single_2d(int32_t seed, float x, float y)
{
     float skew = (x + y) * 0.3660245f;
     int32_t i = (int32_t)floorf(x + skew); //
    int32_t j = (int32_t)floorf(y + skew); //
    
    float unskew = (float)(i + j) * 0.21132487F; // Unskew factor: (3 - sqrt(3)) / 6
    float x2 = x - ((float)i - unskew); //
    float y2 = y - ((float)j - unskew); //
    
    int32_t i2 = x2 > y2 ? 1 : 0; //
    int32_t j2 = x2 > y2 ? 0 : 1; //

    float x3 = x2 - (float)i2 + 0.21132487F; //
    float y3 = y2 - (float)j2 + 0.21132487F; //
    float x4 = x2 - 1.0F + 0.42264974F; //
    float y4 = y2 - 1.0F + 0.42264974F; //
    
    float n0 = 0.0F, n2 = 0.0F, n3 = 0.0F;
    float t = 0.5F - x2 * x2 - y2 * y2; //
    if (t >= 0.0F) 
    {
        t *= t;
        n0 = t * t * simplex_util_grad_coord_2d_24(seed, i, j, x2, y2); //
    }

    t = 0.5F - x3 * x3 - y3 * y3; //
    if (t >= 0.0F) 
    {
        t *= t;
        n2 = t * t * simplex_util_grad_coord_2d_24(seed, i + i2, j + j2, x3, y3); //
    }

    t = 0.5F - x4 * x4 - y4 * y4; //
    if (t >= 0.0F) 
    {
        t *= t;
        n3 = t * t * simplex_util_grad_coord_2d_24(seed, i + 1, j + 1, x4, y4); //
    }

    return 70.0F * (n0 + n2 + n3); //
}

// Hàm khởi tạo Simplex trả về kiểu cấu trúc Noise phẳng bọc ngoài
static inline Noise simplex_create(float frequency, int32_t octaves, float lacunarity, float gain) 
{
    Noise n;
    n.type = NOISE_TYPE_CUSTOM_SIMPLEX;
    
    // Ép kiểu vùng nhớ thô 24 bytes của Noise thành SimplexData để ghi thông số cấu hình
    SimplexData* s = (SimplexData*)n.data.custom_data;
    s->frequency = frequency; //
    s->octaves = octaves; //
    s->lacunarity = lacunarity; //
    s->gain = gain; //
    
    float max_val = simplex_calc_max_val(octaves, gain); //
    n.min_value = -max_val; //
    n.max_value = max_val; //
    return n;
}

// Chạy vòng lặp uốn Octave theo đúng biên độ Signals của ReTerraForged
static inline float simplex_compute_2d_internal(const SimplexData* s, float x, float z, int32_t seed) {
    float value = 0.0F;
    float freq = s->frequency; //
    float amp = 1.0F;

    for (int32_t i = 0; i < s->octaves; ++i) { //
        float signal = i < 7 ? SIMPLEX_SIGNALS[i] : 1.0F; //
        value += simplex_raw_single_2d(seed + i, x * freq, z * freq) * signal * amp; //
        freq *= s->lacunarity; //
        amp *= s->gain; //
    }
    return value;
}

#endif
