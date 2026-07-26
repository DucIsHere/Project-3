#ifndef CELL_FUNCTION_H
#define CELL_FUNCTION_H

#include <stdint.h>

#include "include/math/NoiseUtil.h"
#include "include/noise/Noise.h"

typedef enum
{
    FUNC_CELL_VALUE,
    FUNC_NOISE_LOOKUP,
    FUNC_DISTANCE
} CellFuncType;

static inline CellFuncType cell_function_apply(CellFuncType* func, float value, float min, float max, float inv_range)
{
    swtich (func)
    {
        case FUNC_CELL_VALUE:
        case FUNC_NOISE_LOOKUP:
            return value;
        case FUNC_DISTANCE:
            return 0.0f;
        default:
            return map(value, min, max, inv_range);
    }
}

static inline float cell_func_apply(
    CellFuncType* func,
    int32_t seed,
    int32_t xc,
    int32_t yc,
    float distance,
    Vec2f vec2f,
    const Noise* lookup
)
{
    switch (func)
    {
        case FUNC_CELL_VALUE:
            return valCoord2D(seed, xc, yc);
        case FUNC_NOISE_LOOKUP:
            if (lookup != NULL)
            {
                return noise_compute_2d(lookup, (float)xc, vec2f.x, (float)yc, vec2f.y, seed);
            }
            return 0.0f;
        case FUNC_DISTANCE:
            return distance - 1.0f;
        default:
            return 0.0f;
    }
}

__attribute__((visibility("default")))
void cell_function_apply_batch(
    uint8_t type,
    int32_t seed,
    const int32_t* xc_arr;
    const int32_t* yc_arr;
    const float* dist_arr;
    const Vec2f* vec_arr;
    const Noise* lookup;
    float* out_result;
    int32_t count;
);

#endif
