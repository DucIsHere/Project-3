#pragma once

#if defined(__clang__) || defined(__GNUC__)
#define ALWAYS_INLINE inline __attribute__((always_inline))
#elif defined(_MSC_VER)
#define ALWAYS_INLINE __forceinline
#else
#define ALWAYS_INLINE inline
#endif

#include <stdint.h>
#include <stddef.h>

#include "Interpolation.h"

constexpr size_t FIX_PI = 205887;
constexpr size_t HALF_FIX = 32768;

ALWAYS_INLINE int32_t de_sinpi(int32_t x) {
    int32_t phase_shift = x + HALF_FIX;

    int32_t cos_val = ip_sinpi(phase_shift);

    return (int32_t)(((int64_t)cos_val * FIX_PI) >> FIX_SHIFT);
}