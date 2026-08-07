#ifndef LAKE_H
#define LAKE_H

#if defined(__GNUC__) || defined(__clang__)
    #pragma GCC optimize("03,unroll-loops,fast-math")
    #pragma GCC target("avx2,fma")
    #define RESTRICT __restrict__
    #define ALWAYS_INLINE inline __attribute__((always_inline))
    #define ALIGN_SIMD __attribute__((aligned(SIMD_ALIGN)))
#elif defined(_MSC_VER)
    #pragma optimize("gt, on")
    #define RESTRICT __restrict
    #define ALWAYS_INLINE __forceinline
    #define ALIGN_SIMD __declspec(align(SIMD_ALIGN))
#endif

#include <stdint.h>
#include <stdbool.h>

#include "cell/Cell.h"
#include "terrain/TerrainType.h"
#include "include/math/NoiseUtil.h"
#include "include/util/Boundsf.h"

typedef struct ALIGN_SIMD
{
    float center_x;
    float center_y;
    float lakeDistance2;
    float valley2;
    float valleyDistance2;
    float depth;
    float bankMin;
    float bankMax;
    float bankAlphaMin;
    float bankAlphaRange;
    float invBankAlphaRange;
} LakeNative;

static ALWAYS_INLINE void lake_apply_soa(
    const LakeNative* RESTRICT lake,
    CellSoA* RESTRICT soa,
    const float* RESTRICT xs,
    const float* RESTRICT zs,
    int count
) 
{
    // Unpack thuộc tính Lake ra Local Variables để Compiler giữ cố định trên Register
    const float cx = lake->center_x;
    const float cz = lake->center_y;
    const float v2 = lake->valley2;
    const float lDist2 = lake->lakeDistance2;
    const float vDist2 = lake->valleyDistance2;
    const float lDepth = lake->depth;
    const float bMin = lake->bankMin;
    const float bMax = lake->bankMax;
    const float bAlphaMin = lake->bankAlphaMin;
    const float invbAlphaRange = lake->invBankAlphaRange;

    // Direct Pointers từ SoA (Zero Indirection)
    float* RESTRICT height = soa->height;
    float* RESTRICT river_mask = soa->river_mask;
    uint8_t* RESTRICT terrain = soa->terrain;

    #if defined(__GNUC__) || defined(__clang__)
        #pragma clang loop vectorize(enable) interleave(enable)
    #endif
    for (int i = 0; i < count; ++i) 
    {
     float dx = cx - xs[i];
     float dz = cz - zs[i];
     float distance2 = dx * dx + dz * dz;

        // Bỏ qua nếu nằm ngoài bán kính thung lũng (Valley Radius)
     if (distance2 > v2) continue;

        // Tính Bank Height dùng Fast Map/Lerp
     float h_curr = height[i];
     float bankHeightAlpha = map_inv_range(h_curr, bAlphaMin, bMax, invbAlphaRange);
     float bankHeight = lerpf_fast(bMin, bMax, bankHeightAlpha);

        // --- 1. VÙNG LÒNG HỒ (LAKE CORE) ---
     if (distance2 <= lDist2) 
        {
          if (bankHeight < h_curr) 
          {
               h_curr = bankHeight;
               height[i] = h_curr;
          }

          if (distance2 < lDist2) 
          {
               float depthAlpha = clampf_fast(1.0f - (distance2 / lDist2), 0.0f, 1.0f);
               float targetDepth = h_curr < lDepth ? h_curr : lDepth;
                
               height[i] = lerpf_fast(h_curr, targetDepth, depthAlpha);
               terrain[i] = (uint8_t)LAKE; // Set ID = 6 (LAKE) từ TerrainType.h[cite: 3]
                
               float invDepthAlpha = 1.0f - depthAlpha;

               if (invDepthAlpha < river_mask[i]) 
               {
                  river_mask[i] = invDepthAlpha;
               }
          }
          continue;
     }

        // --- 2. VÙNG BỜ/THUNG LŨNG (VALLEY BORDER) ---
        if (h_curr >= bankHeight) {
            float valleyAlpha = clampf_fast(1.0f - (distance2 - lDist2) / vDist2, 0.0f, 1.0f);[cite: 2]
            height[i] = lerpf_fast(h_curr, bankHeight, valleyAlpha);[cite: 2]
            
            float invValleyAlpha = 1.0f - valleyAlpha;
            if (invValleyAlpha < river_mask[i]) {
                river_mask[i] = invValleyAlpha;
            }
        }
    }
}

#endif
