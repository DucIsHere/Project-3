#ifndef DISTANCE_FUNCTION_H
#define DISTANCE_FUNCTION_H

#include <stdint.h>

#include "include/math/NoiseUtil.h"

typedef enum
{
    DISTANCE_FUNC_ECULIDEAN = 0,
    DISTANCE_FUNC_MANHATTAN = 1,
    DISTANCE_FUNC_NATURAL = 2
} DistanceFunc;

static inline float distance_apply(DistanceFunc func, float x, float y)
{
    switch (func)
    {
        case DISTANCE_FUNC_ECULIDEAN:
            return x * x + y * y;
        case DISTANCE_FUNC_MANHATTAN:
            return abs(x) + abs(y);
        case DISTANCE_FUNC_NATURAL:
            return abs(x) + abs(y) + (x * x + y * y);
        default: 
            return 0.0f;
    }
}

#endif
