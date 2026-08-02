#ifndef SIN_H
#define SIN_H

#include <stdint.h>
#include <stdlib.h>

#include "include/math/NoiseUtil.h"
#include "include/noise/Noise.h"

typedef struct 
{
    Noise* alpha;
    float frequency;
} SinData;

static inline Noise sin_compute(Noise* self, float x, float z, int32_t seed)
{
    if (data == NULL) return 0.0f;

    float a = 0.0f;
    
    if (data->alpha != NULL)
    {
        a = noise_2d(data->alpha, x, z, seed);
    }

    x *= data->frequency;
    z *= data->frequency;

    float noise;

    if (a == 0.0f)
    {
        noise = usin(x);
    }
    else if (a == 1.0f)
    {
        noise = usin(z);
    }
    else 
    {
        float sx = sin(x);
        float sy = sin(z);
        noise = lerp(sx, sy, a);
    }
    return map(noise, -1.0f, 1.0f, 2.0f);

}

static inline Noise sin_create(float frequency, Noise* alpha)
{
    Noise n;
    n.type = NOISE_TYPEDEF_SIN;
    n.node_ref = NULL;

    n.min_value = 0.0f;
    n.max_value = 1.0f;

    SinData* data = (SinData*).n.data.custom_data;
    data->frequency = frequency;
    data->alpha = alpha;

    return n;
}

#endif
