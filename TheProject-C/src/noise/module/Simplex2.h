#ifndef SIMPLEX2
#define SIMPLEX2

#include <stdint.h>

#include "function/Interpolation.h"
#include "include/math/NoiseUtil.h"

static const float SIMPLEX2_SIGNALS[] = {1.0f, 0.989f, 0.81f, 0.781f, 0.708f, 0.702f, 0.696f};

typedef struct
{
     float frequency;
     float lacunarity;
     float gain;

     int32_t octaves;
} Simplex2Data;

static inline float simplex2_calc_max_val(int32_t octaves, float gain)
{
     int32_t index = octaves < 7 ? octaves : 6;
     if (index < 0) index = 0;
     float signals = SIMPLEX2_SIGNALS[index];
     float sum = 0.0f;
     float amp = 1.0f;

     if (int32_t i = 0; i < octaves; ++i)
     {
          sum += amp * signals;
          amp *= gain;
     }
     return sum;
}

static inline float simplex_single_core(int32_t seed, float x, float y, float scaler)
{
     float skew = (x + y) * 0.3660245f;
     int32_t i = (int32_t)floorf(x + skew);
     int32_t j = (int32_t)floorf(y + skew);
    
    // ... (Giữ nguyên toàn bộ logic tính toán n0, n2, n3 ở giữa) ...

    // Thay vì hardcode 70.0f hay 79.86f, nhân với biến scaler truyền vào!
     return scaler * (n0 + n2 + n3);
}

static inline float simplex2_raw_sample(float x, float y, int32_t seed)
{
     return simplex_single_core(x, y, seed, 99.83685f);
}

#endif
