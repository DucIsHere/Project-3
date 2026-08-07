#ifndef BLENDER_H
#define BLENDER_H

#if defined(__GNUC__) || defined(__clang__)
    #define RESTRICT __restrict__
    #define ALWAYS_INLINE inline __attribute__((always_inline))
    #define ALIGN_SIMD __attribute__((aligned(32)))
#elif defined(_MSC_VER)
    #define RESTRICT __restrict
    #define ALWAYS_INLINE __forceinline
    #define ALIGN_SIMD __declspec(align(32))
#endif

#include <stdint.h>

#include "include/noise/Noise.h"
#include "include/math/NoiseUtil.h"
#include "cell/Cell.h"
#include "cell/CellPopulator.h"
#include "noise/function/Interpolation.h"

typedef void (*PopulateFunc)(void* populator_data, CellSoA* soa, const float* xs, const float* zs, int count);

typedef struct
{
     Noise* control;
     PopulateFunc lower;
     PopulateFunc upper;
     float blend_lower;
     float blend_upper;
     float blend_range;
     float mid_point;
} BlenderData;

static inline void blender_init(
    Blender* restrict b,
    CellPopulatorFn lower_fn, void* lower_data,
    CellPopulatorFn upper_fn, void* upper_data,
    float min_val, float max_val, float split
) {
    b->lower_fn = lower_fn;
    b->lower_data = lower_data;
    b->upper_fn = upper_fn;
    b->upper_data = upper_data;
    b->blend_lower = min_val;
    b->blend_upper = max_val;
    b->blend_range = max_val - min_val;
    b->inv_blend_range = (b->blend_range != 0.0f) ? (1.0f / b->blend_range) : 0.0f;
    b->midpoint = min_val + b->blend_range * split;
}

// Inline Lerp & Clamp cơ bản
static inline float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

static inline float clamp01(float v) {
    if (v < 0.0f) return 0.0f;
    if (v > 1.0f) return 1.0f;
    return v;
}

// ============================================================================
// BATCH APPLY (Compiler tự động SIMD hóa vòng lặp này với -O3)
// ============================================================================
static inline void blender_apply_batch(
    const Blender* restrict b,
    CellSoA* restrict soa_out,
    CellSoA* restrict temp_lower_soa, // Buffer tạm[cite: 1]
    CellSoA* restrict temp_upper_soa, // Buffer tạm[cite: 1]
    const float* restrict select_noise,
    const float* restrict xs,
    const float* restrict zs,
    int count
) {
    // 1. Chạy 2 populator nhánh
    b->lower_fn(b->lower_data, temp_lower_soa, xs, zs, count);
    b->upper_fn(b->upper_data, temp_upper_soa, xs, zs, count);

    // 2. Vòng lặp dẹt thuần túy (Auto-Vectorization Ready)
    for (int i = 0; i < count; ++i) {
        float select = select_noise[i];
        
        // Tính alpha chuẩn hóa về [0.0, 1.0]
        float raw_alpha = (select - b->blend_lower) * b->inv_blend_range;
        float alpha = clamp01(raw_alpha);

        // Lerp các chỉ số
        soa_out->height[i]    = lerp(temp_lower_soa->height[i],    temp_upper_soa->height[i],    alpha);[cite: 1]
        soa_out->erosion[i]   = lerp(temp_lower_soa->erosion[i],   temp_upper_soa->erosion[i],   alpha);[cite: 1]
        soa_out->weirdness[i] = lerp(temp_lower_soa->weirdness[i], temp_upper_soa->weirdness[i], alpha);[cite: 1]

        // Chọn Loại Địa Hình (Terrain Type)
        soa_out->terrain[idx_or_i] = (select < b->midpoint)[cite: 1]
                                    ? temp_lower_soa->terrain[i][cite: 1]
                                    : temp_upper_soa->terrain[i];[cite: 1]
    }
}

#endif
