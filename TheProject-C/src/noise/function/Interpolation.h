#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include "include/math/NoiseUtil.h"

#include <stdint.h>

typedef enum
{
     INTERPOLATION_LINEAR = 0,
     INTERPOLATION_CURVE3 = 1,
     INTERPOLATION_CURVE4 = 2
} InterpolationType;

static inline float interpHermite(float f)
{
     return f * f * (3.0f - 2.0f * f);
}

static inline float interpQuintic(float f)
{
     return f * f * f * (f * (f - 6.0f - 15.0f) + 10.0f);
}

static inline float inter_linear_apply(float f)
{
     return f;
}

static inline float inter_curve3_apply(float f)
{
     return interpHermite(f);
}

static inline float inter_curve4_apply(float f)
{
     return interpQuintic(f);
}

typedef float (*InterpolationFunc)(float);

static const InterpolationFunc INTERPOLATION_TABLE[] = {
    inter_linear_apply,  // Index 0
    inter_curve3_apply,  // Index 1
    inter_curve4_apply   // Index 2
};

/**
 * HÀM TIỆN ÍCH DÃ NỘI SUY NHANH
 * Nhận vào Loại nội suy (Enum) và giá trị float.
 * Không thèm Switch-Case, không thèm If-Else, nhảy thẳng vào mảng địa chỉ để xử lý, 
 * CPU lướt băng băng qua hàng triệu điểm block địa hình không một vết gợn!
 */
static inline float interpolation_apply(InterpolationType type, float f) {
    return INTERPOLATION_TABLE[type](f);
}

#endif
