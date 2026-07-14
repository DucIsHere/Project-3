#ifndef SIMPLEX2_H
#define SIMPLEX2_H

#include <stdint.h>
#include <math.h>

#include "Noise.h" 
#include "include/noise/Noise.h"
#include "include/math/NoiseUtil.h"

static const float SIMPLEX2_SIGNALS[] = { 1.0F, 0.989F, 0.81F, 0.781F, 0.708F, 0.702F, 0.696F };

typedef struct {
    float frequency;
    float lacunarity;
    float gain;
    int32_t octaves;
    float min_val;
    float max_val;
} Simplex2Data;
// Ma trận xoay góc gradient mặc định của Simplex gốc (Mã hóa bitwise)
static inline float simplex2_native_grad(int32_t hash, float x, float y) {
    int32_t h = hash & 7; 
    float u = h < 4 ? x : y;
    float v = h < 4 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0F * v : 2.0F * v);
}

// Khớp chuẩn xác thuật toán tính toán biên độ max dựa theo tổng octaves của Simplex2.java
static inline float simplex2_calc_max(int32_t octaves, float gain) {
    int32_t index = octaves < 7 ? octaves : 6; // Khớp Math.min(octaves, SIGNALS.length - 1)
    if (index < 0) index = 0;
    float signal = SIMPLEX2_SIGNALS[index]; 
    
    float sum = 0.0F;
    float amp = 1.0F;
    for (int32_t i = 0; i < octaves; ++i) {
        sum += amp * signal; 
        amp *= gain;
    }
    return sum;
}

// Hàm băm thô tầng đơn khớp trực tiếp với Simplex.singleSimplex và giữ nguyên số hardcode
static inline float simplex2_raw_single_2d(int32_t seed, float x, float y) {
    float n0 = 0.0F, n2 = 0.0F, n3 = 0.0F; 
    float t;

    float skew = (x + y) * 0.366025403F;
    int32_t i = simplex2_noise_util_floor(x + skew);
    int32_t j = simplex2_noise_util_floor(y + skew);

    float unskew = (i + j) * 0.211324865F;
    float x2 = x - (i - unskew);
    float y2 = y - (j - unskew);

    int32_t i2 = (x2 > y2) ? 1 : 0;
    int32_t j2 = (x2 > y2) ? 0 : 1;

    float x3 = x2 - i2 + 0.211324865F;
    float y3 = y2 - j2 + 0.211324865F;
    float x4 = x2 - 1.0F + 0.422649731F;
    float y4 = y2 - 1.0F + 0.422649731F;

    t = 0.5F - x2 * x2 - y2 * y2;
    if (t >= 0.0F) {
        t *= t;
        int32_t hash = simplex2_hash_2d(seed, i, j); 
        n0 = t * t * simplex2_native_grad(hash, x2, y2); 
    }
    
    t = 0.5F - x3 * x3 - y3 * y3;
    if (t >= 0.0F) {
        t *= t;
        int32_t hash = simplex2_hash_2d(seed, i + i2, j + j2);
        n2 = t * t * simplex2_native_grad(hash, x3, y3);
    }

    t = 0.5F - x4 * x4 - y4 * y4;
    if (t >= 0.0F) {
        t *= t;
        int32_t hash = simplex2_hash_2d(seed, i + 1, j + 1);
        n3 = t * t * simplex2_native_grad(hash, x4, y4);
    }

    return 99.83685F * (n0 + n2 + n3); 
}

// Khớp 100% logic vòng lặp đa tầng và ánh xạ dải điểm của Simplex2.java
static inline float simplex2_compute_2d_internal(const Simplex2Data* s, float x, float z, int32_t seed) {
    x *= s->frequency;
    z *= s->frequency;
    float sum = 0.0F;
    float amp = 1.0F;

    for (int32_t i = 0; i < s->octaves; ++i) {
        sum += simplex2_raw_single_2d(seed + i, x, z) * amp;
        x *= s->lacunarity;
        z *= s->lacunarity;
        amp *= s->gain;       
    }
    
    float range = s->max_val - s->min_val;
    return simplex2_noise_util_map(sum, s->min_val, s->max_val, range); 
}

// Factory khởi tạo cấu hình tương đương cấu trúc dữ liệu constructor trong Java
static inline Noise noises_simplex2_create(float frequency, int32_t octaves, float lacunarity, float gain) {
    Noise n;
    n.type = NOISE_TYPEDEF_SIMPLEX2;        
    n.node_ref = NULL; 
    n.min_value = 0.0F; // KhớpminValue() -> 0.0F
    n.max_value = 1.0F; // Khớp maxValue() -> 1.0F
    
    Simplex2Data* s = (Simplex2Data*)n.data.custom_data;
    s->frequency = frequency;
    s->octaves = octaves;
    s->lacunarity = lacunarity;
    s->gain = gain;
    
    float max_bound = simplex2_calc_max(octaves, gain);
    s->max_val = max_bound;  
    s->min_val = -max_bound; 
    
    return n;
}

#endif
