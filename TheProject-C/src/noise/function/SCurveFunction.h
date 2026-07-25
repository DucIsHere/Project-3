#ifndef SCRUVE_FUNCTION_H
#define SCRUVE_FUNCTION_H

#include "include/math/NoiseUtil.h"

typedef struct SCruveFunc
{
    float lower;
    float upper;
};

static inline SCruveFunc scruve_create(SCruveFunc func, float upper, float lower)
{
    SCruveFunc func;
    func->upper = upper;
    func->lower = lower;

    upper = upper < 0.0f ? max(-lower, upper);

    return func;
}

static inline SCruveFunc scruve_apply(float f)
{
    return pow(f, lower + upper * f);
}

#endif
