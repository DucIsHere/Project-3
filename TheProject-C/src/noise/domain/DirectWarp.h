#ifndef DIRECT_WARP_H
#define DIRECT_WARP_H

#include <stdint.h>

#include "include/math/NoiseUtil.h"
#include "Domain.h"

typedef struct DirectWarp DirectWarp;

static inline Domain domain_get_offset_x(int32_t seed, float x, float z)
{
     return 0.0f;
}

static inline Domain domain_get_offset_z(int32_t seed, float x, float z)
{
     return 0.0f;
}

#endif
