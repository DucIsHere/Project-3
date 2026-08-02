#ifndef MATRIX_3D_FRAGMENT_H
#define MATRIX_3D_FRAGMENT_H

#include <stdint.h>
#include <immintrin.h>

#include "Matrix.h"

// ==========================================================
// 1. CẤU TRÚC MẢNH 1D THEO TRỤC X (X-FIBER FRAGMENTS)
// ==========================================================

typedef struct ALIGN32
{
    float x[2];
    uint8_t y, z;
    uint32_t bitmask;
} FragX2;

typedef struct ALIGN32
{
    float x[3];
    uint8_t y, z;
    uint32_t bitmask;
} FragX3;

// ==========================================================
// 2. HELPER: TÍNH BIT MASK MẢNH (1 << Index)
// Mỗi mảnh (y, z) được gán 1 bit flag unique trong không gian YZ
// ==========================================================

static inline uint8_t frag_calc_bitmask(uint8_t y, uint8_t z, uint8_t max_y)
{
    uint8_t fiber_index = (z * max_y) + y;
    return (1U << fiber_index);
}

#endif