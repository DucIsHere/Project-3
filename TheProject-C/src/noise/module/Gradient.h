#ifndef GRADIENT_H
#define GRADIENT_H

#include <stdint.h>

#include "include/math/NoiseUtil.h"
#include "include/noise/Noise.h"

typedef struct 
{
     const Noise input;
     const Noise lower;
     const Noise upper;
     const Noise strength;
} Grad;

static inline Noise grad_in(Noise* i, Noise* l, Noise* u, Noise* s)
{
     Noise n;
     n.type = NOISE_TYPEDEF_GRADIENT;
     n.node_ref = NULL;

     float min = noise_get_min_value(i);
     float max = noise_get_max_value(i);

     Grad* g = (Grad*)n.data.custom_data;
     g->input = i;
     g->lower = l;
     g->upper = u;
     g->strength = s; 

     return n;
}

static inline float grad_compute(float x, float z, int32_t seed)
{
     float noise_value = noise_compute_2d(g->i, x, z, seed);
     float upper_bound = noise_compute_2d(g->u, x, z, seed);
     if (noise_value > upper_bound) return noise_value;

     float amount = noise_compute_2d(g->s, x, z, seed);
     float lower_bound = noise_compute_2d(g->l, x, z, seed);
     if (noise_value < lower_bound) 
     {
          return pow(noise_value, 1.0f - amount);
     }

     float alpha = 1.0f - (noise_value - lower_bound) / (upper_bound - lower_bound);
     float power = 1.0f - amount * alpha;
     return pow(noise_value, power);
}

#endif
