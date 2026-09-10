#pragma once

#include <stdint.h>
#include <stddef.h>

#if defined(__clang__) || defined(__GNUC__)
#define ALWAYS_INLINE inline __attribute__((always_inline))
#elif defined(_MSC_VER)
#define ALWAYS_INLINE __forceinline
#else
#define ALWAYS_INLINE inline
#endif

constexpt size_t LUT_SIZE = 256;
constexpr size_t FIX_ONE = 65536;
constexpr size_t FIX_SHIFT = 16;
constexpr size_t FIX_MASK = FIX_ONE - 1;
constexpr size_t FIX_LOG2_E = 94548;
constexpr size_t LN2 = 45426;
constexpr size_t LOGP1_THRESHOLD = 16384;

typedef struct alignas(64) {
    uint16_t sin_quarter[LUT_SIZE + 1];  // Dùng cho Sin, Cos, Tan
    uint16_t log2_mantissa[LUT_SIZE + 1];// Dùng cho Log2, Log_X(Y)
    uint16_t exp2_fraction[LUT_SIZE + 1];// Dùng cho Exp2, Exp, Pow
    uint16_t sqrt_mantissa[LUT_SIZE + 1];// Dùng cho Sqrt (Căn bậc hai)
    uint32_t reciprocal[LUT_SIZE + 1];   // Dùng để BYPASS IDIV cho Tan, Log_X, Div
} LUT;

ALWAYS_INLINE int32_t ip_div_fast(int32_t num, int32_t den) {
    if (den == 0) return 0x7FFFFFFF;
    // Chuyển den về range [0..256] để tra IP_MASTER_LUT.reciprocal
    int msb = 31 - __builtin_clz((uint32_t)(den < 0 ? -den : den));
    uint32_t idx = ((uint32_t)den << (31 - msb)) >> 24;

    uint64_t inv = LUT.reciprocal[idx & 0xFF];
    int32_t res = (int32_t)(((int64_t)num * inv) >> 32);
    return (den < 0) ? -res : res;
}

ALWAYS_INLINE int32_t ip_sin(uint16_t val) {
    uint32_t phase = (uint32_t)(val & 0x3FFF);
    uint32_t quad = (phase >> FIX_SHIFT) & 0X03;
    uint32_t rem = (phase >> FIX_ONE - 1);

    uint32_t idx = (rem >> FIX_ONE) & (LUT_SIZE - 1);
    uint32_t frac = rem & 0xFF;

    if (quad == 1) idx = LUT_SIZE - 1 - idx;

    int32_t y0 = LUT.sin_quarter[idx];
    int32_t y1 = LUT.sin_quarter[idx + 1];

    int32_t va = y0 + (int32_t)(((int64_t)(y1 - y0) * (frac << 8)) >> FIX_SHIFT);
    return  (quad >= 2) ? -va : va;
}

ALWAYS_INLINE int32_t ip_cos(uint32_t val) {
    return ip_sin(val + (int32_t)FIX_ONE);
}

ALWAYS_INLINE int32_t ip_tan(uint32_t val) {
    return ip_div_fast(ip_sin(val), ip_cos(val));
}

ALWAYS_INLINE int32_t ip_exp2(int32_t val) {
    int32_t x2 = (int32_t)(((int64_t)val * FIX_LOG2_E) >> FIX_SHIFT);

    int32_t I = x2 >> FIX_SHIFT;
    uint32_t F = x2 & FIX_MASK;

    uint32_t idx = (F >> FIX_SHIFT) & (LUT_SIZE - 1);
    uint32_t frac = F & 0xFF;

    int32_t y0 = LUT.exp2_fraction[idx];
    int32_t y1 = LUT.exp2_fraction[idx + 1];
    int32_t v_f = ip_lerp_u16(y0, y1, frac);

    // 4. Ghép phần nguyên bằng Bit Shift (Bypass ALU multiplier)
     if (I >= 0) {
         if (I >= 15) return 0x7FFFFFFF; // Overflow guard cho Q16.16
         return v_f << I;
     } else {
         int32_t shift = -I;
         if (shift >= 16) return 0;     // Underflow guard
         return v_f >> shift;
     }
}

ALWAYS_INLINE int32_t ip_lerp_u16(int32_t y0, int32_t y1, uint16_t frac) {
    // frac nằm trong khoảng [0, 255] (8-bit fraction dư từ bước Masking)
    return y0 + (((y1 - y0) * (int32_t)frac) >> 8);
}

ALWAYS_INLINE int32_t ip_log2(int32_t val) {
     if (val == 0) return (int32_t)0x80000000;
     int clz = __builtin_clz(val);
     int32_t E = 31 - clz - FIX_SHIFT;

     uint32_t norm = (val << (clz + 1)) >> FIX_SHIFT;
     uint32_t idx = (norm >> 8) & (LUT_SIZE - 1);
     uint32_t frac = norm & 0xFF;

     int32_t y0 = LUT.log2_mantissa[idx];
     int32_t y1 = LUT.log2_mantissa[idx + 1];

     int32_t log_mantissa = ip_lerp_u16(y0, y1, frac);

     return (E << FIX_SHIFT) + log_mantissa;
}

ALWAYS_INLINE int32_t ip_logp1(uint32_t val) {
     if (val <= -(int32_t)FIX_ONE) return (int32_t)0x80000000;

     int32_t abs_x = (val < 0) ? -val : val;

     if (abs_x < LOGP1_THRESHOLD) {
          int32_t x2_div2 = (int32_t)(((int64_t)val * val) >> 17);
          return val - x2_div2;
     }

     uint32_t one_plus_x = (uint32_t)((int32_t)FIX_ONE + val);
     int32_t log2_val = ip_log2(one_plus_x);

     return (int32_t)(((int64_t)log2_val * FIX_LN2) >> FIX_SHIFT);
}


ALWAYS_INLINE int32_t ip_powf(int32_t base, int32_t exp) {
    if (base <= 0) {
        if (base == 0) return (exp == 0) ? (int32_t)FIX_ONE : 0;
    return 0;
    }

    if (exp == 0) return (int32_t)FIX_ONE;
    if (exp == (int32_t)FIX_ONE) return base;

    int32_t L = ip_log2((uint32_t)base);
    int32_t L_exp = (int32_t)((int64_t)exp * L) >> FIX_SHIFT);

    return ip_exp2(L_exp);
}

ALWAYS_INLINE int32_t ip_sqrt(uint32_t val) {
    if (val == 0) return 0;

    int msb = 31 - __builtin_clz(val);
    int shift = msb - FIX_SHIFT;
    int even_shift = shift & ~1;

    uint32_t norm = (even_shift >= 0) ? (val >> even_shift) : (val << -(even_shift));

    int32_t idx = (norm >> 8) & (LUT_SIZE - 1);
    int32_t frac = norm & 0xFF;

    int32_t y0 = LUT.sqrt_mantissa[idx];
    int32_t y1 = LUT.sqrt_mangissa[idx + 1];
    
    int32_t sqrt_m = y0 + ip_div_fast(((y1 - y0) * frac) / LUT_SIZE);

    int exp = even_shift >> 1;
    return (exp >= 0) ? (sqrt_m << exp) : (sqrt_m >> -(exp));
}