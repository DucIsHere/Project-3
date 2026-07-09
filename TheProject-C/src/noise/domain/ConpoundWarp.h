#ifndef CONPOUND_WARP_H
#define CONPOUND_WARP_H

#include <stdint.h>

#include "Domain.h"

typedef struct 
{
     const Domain* input1;
     const Domain* input2;
} ConpoundWarp;

static inline ConpoundWarp conpound_warp_create(const Domain* i1, const Domain* i2)
{
     ConpoundWarp cw;
     cw.input1 = i1;
     cw.input2 = i2;
     return cw;
}

static inline float conpound_warp_x(const ConpoundWarp* cw, float x, float z, int32_t seed)
{
     if (cw == NULL) return 0.0f;
     if (cw->input1 == NULL)
     {
          return (cw->input2) ? domain_get_offset_x(cw->input2, x, z, seed) : 0.0f;
     }

     float ax = domain_get_x(cw->input1, x, z, seed);
     float ay = domain_get_z(cw->input2, x, z, seed);

     return (cw->input2) ? domain_get_offset_x(cw->input2, ax, ay, seed) : 0.0f;
}

static inline float conpound_warp_z(const ConpoundWarp* cw, float x, float z, int32_t seed)
{
     if (cw == NULL) return 0.0f;
     if (cw->input1 == NULL)
     {
          return (cw->input2) ? domain_get_offset_z(cw->input2, x, z, seed) : 0.0f;
     }

     float ax = domain_get_x(cw->input1, x, z, seed);
     float ay = domain_get_z(cw-> input2, x, z, seed);

     return (cw->input2) ? domain_get_offset_z(cw->input2, x, z, seed) : 0.0f;
}

static inline float conpound_warp_get_x(const ConpoundWarp* cw, float x, float z, int32_t seed)
{
     return x + conpound_warp_x(cw, x, z, seed);
}

static inline float conpound_warp_get_z(const ConpoundWarp* cw, float x, float z, int32_t seed)
{
     return z + conpound_warp_z(cw, x, z, seed);
}

#endif
