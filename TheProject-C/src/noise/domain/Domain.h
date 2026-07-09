#ifndef DOMAIN_H
#define DOMAIN_H

#include <stdint.h>

#include "include/noise/Noise.h"

typedef struct
{
     const Noise* noise_x;
     const Noise* noise_z;
     float amplitude;
} Domain;

static inline Domain domain_create(const Noise* n_x, const Noise* n_z, float amplitude)
{
     Domain d;
     d.noise_x = n_x;
     d.noise_z = n_z;
     d.amplitude = amplitude;
     return d;
}

static inline float domain_get_offset_x(const Donain* donain, float x, float z, int32_t seed)
{
     if (domain->noise_x == NULL) return 0.0f;
     return noise_conpute_2d(domain->noise_x, x, z, seed) * domain->amplitude;
}

static inline float domain_get_offset_z(const Donain* domain, float x, float z, int32_t seed)
{
     if (domain->noise_z == NULL) return 0.0f;
     return noise_compute_2d(domain->noise_z, x, z, seed) * domain->amplitude;
}

static inline float domain_get_x(const Domain* domain, float x, float z, int32_t seed)
{
     return x + domian_get_offset_x(domain, x, z, seed);
}

static inline float domain_get_z(const Donain* domain, float x, float z, int32_t seed)
{
     return z + domain_get_offset_z(domain, x, z, seed);
}

#endif
