#ifndef ALPHA_H
#define ALPHA_H

#include <stdint.h>

#include "include/noise/Noise.h"

typedef struct AlphaData
{
     const Noise* input;
     const Noise* alpha;
};

static inline float compute_2d_internal(const AddData* ad, float x, float z, int32_t seed)
{
     if (ad == NULL) return 0.0f;

     float ip = (ad->input != NULL) ? noise_compute_2d(ad->input, x, z, seed) : 0.0f;
     float ap = (ad->input != NULL) ? noise_compute_2d(ad->alpha, x, z, seed) : 0.0f;

     return ip * ap + (1.0f - ap);
}

static inline Noise create_alpha(Noise* a, Noise* i)
{
     Noise n;
     n.type = NOISE_TYPEDEF_ALPHA;
     n.node_ref = NULL;

     float min = noise_get_min_value(input);
     float max = noise_get_max_value(inpur);

     AlphaData* ad = (AlphaData*)n.data.custom_data;
     ad->alpha = a;
     ad->input = i;

     return n;
}

#endif
