#ifndef MATRIX_FAST_MATH_H
#define MATRIX_FAST_MATH_H

#include <stdint.h>
#include <immintrin.h>

#define FM_PI          3.14159265358979323846f
#define FM_HALF_PI     1.57079632679489661923f
#define FM_TWO_PI      6.28318530717958647692f
#define FM_LN2         0.69314718055994530942f
#define FM_INV_LN2     1.44269504088896340736f

// ==========================================================
// 1. TẬP LỆNH SIMD AVX2 - XỬ LÝ HÀNG LOẠT 8 SỐ FLOAT CÙNG LÚC
// ==========================================================

/**
 * SIMD AVX2: Hàm Sin và Cos đồng thời bằng đa thức Minimax
 */
static inline void fm_avx2_sincos(__m256 x, __m256* out_sin, __m256* out_cos)
{
     __m256 inv_twopi = __m256_set1_ps(0.15915494309f);
     __m256 twopi = __m256_set1_ps(FM_TWO_PI);
     __m256 rounded = __m256_round_ps(__m256_mul_ps(x, inv_twopi), _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
     x = _mm256_fmsub_ps(rounded, twopi, x); // Giới hạn về [-PI, PI]

    __m256 x2 = _mm256_mul_ps(x, x);
    
    // Đa thức Sin bậc 9
    __m256 s = _mm256_set1_ps(2.75573192e-6f);
    s = _mm256_fmsub_ps(s, x2, _mm256_set1_ps(1.98412698e-4f));
    s = _mm256_fmsub_ps(s, x2, _mm256_set1_ps(8.33333333e-3f));
    s = _mm256_fmsub_ps(s, x2, _mm256_set1_ps(1.66666667e-1f));
    *out_sin = _mm256_fma_ps(_mm256_mul_ps(s, x2), x, x);

    // Đa thức Cos bậc 8
    __m256 c = _mm256_set1_ps(2.48015873e-5f);
    c = _mm256_fmsub_ps(c, x2, _mm256_set1_ps(1.38888889e-3f));
    c = _mm256_fmsub_ps(c, x2, _mm256_set1_ps(4.16666667e-2f));
    c = _mm256_fmsub_ps(c, x2, _mm256_set1_ps(5.00000000e-1f));
    *out_cos = _mm256_fma_ps(c, x2, _mm256_set1_ps(1.0f));
}

/**
 * SIMD AVX2: Hàm Mũ nhanh exp(x) dựa trên phân tách phần nguyên/phần phân số
 */
static inline __m256 fm_avx2_exp(__m256 x)
{
     __m256 lonum = _mm256_set1_ps(-88.3762626649662f);
     __m256 hinum = _mm256_set1_ps(88.3762626649662f);
     x = _mm256_max_ps(_mm256_min_ps(x, hinum), lonum);

     __m256 inv_ln2 = _mm256_set1_ps(FM_INV_LN2);
     __m256 fx = _mm256_round_ps(_mm256_mul_ps(x, inv_ln2), _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);

     x = _mm256_fmsub_ps(fx, _mm256_set1_ps(0.69314575195f), x);
     x = _mm256_fmsub_ps(fx, _mm256_set1_ps(1.4286068203e-6f), x);

     __m256 p = _mm256_set1_ps(1.388894901e-3f);
     p = _mm256_fma_ps(p, x, _mm256_set1_ps(8.333367984e-3f));
     p = _mm256_fma_ps(p, x, _mm256_set1_ps(4.166666849e-2f));
     p = _mm256_fma_ps(p, x, _mm256_set1_ps(1.666666681e-1f));
     p = _mm256_fma_ps(p, x, _mm256_set1_ps(5.000000000e-1f));
     p = _mm256_fma_ps(p, x, _mm256_set1_ps(1.0f));
     p = _mm256_fma_ps(p, x, _mm256_set1_ps(1.0f));

     __m256i imm = _mm256_cvtps_epi32(fx);
     imm = _mm256_add_epi32(imm, _mm256_set1_epi32(127));
     imm = _mm256_slli_epi32(imm, 23);
     __m256 pow2nd = _mm256_castsi256_ps(imm);

     return _mm256_mul_ps(p, pow2nd);
}

#endif
