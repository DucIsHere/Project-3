#ifndef EDGE_FUNCTION_H
#define EDGE_CUNCTION_H

#include <stdint.h>

typedef enum
{
    EDGE_FUNC_DISTANCE_2 = 0,
    EDGE_FUNC_DISTANCE_2_ADD = 1,
    EDGE_FUNC_DISTANCE_2_SUB = 2,
    EDGE_FUNC_DISTANCE_2_MUL = 3,
    EDGE_FUNC_DISTANCE_2_DIV = 4
} EdgeFunctionType;

static inline float edge_function_apply(EdgeFunctionType* type, float distance, float diatance2)
{
    switch (type)
    {
        case EDGE_FUNC_DISTANCE_2:
            return distance2 - 1.0f;
        case EDGE_FUNC_DISTANCE_2_ADD:
            return distance2 + distance - 1.0f;
        case EDGE_FUNC_DISTANCE_2_SUB:
            return distance2 - distance - 1.0f;
        case EDGE_FUNC_DISTANCE_2_MUL:
            return distance2 * distance - 1.0f;
        case EDGE_FUNC_DISTANCE_2_DIV:
            return distance / distance2 - 1.0f;
        default: 
            return 0.0f
    }
}

static inline float max_edge(EdgeFunctionType type)
{
    static const float max_table[] = {1.0f, 1.6f, 0.8f, 0.7f, 0.0f}
    return max_table[type];
}

static inline float min_edge(EdgeFunctionType type)
{
    return -1.0f;
}

static inline float range_edge(EdgeFunctionType type)
{
    static const float range_table[] = {2.0f, 2.6f, 1.8f, 1.7f, 1.0f}
    return range_table[type];
}

#endif
