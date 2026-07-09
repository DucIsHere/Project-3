#ifndef ADD_WARP_H
#define ADD_WARP_H

#include "Domain.h"

#include <stdint.h>

typedef struct
{
     const Domain* input1;
     const Domain* input2;
} AddWarp;

static inline AddWarp add_warp_create(const Domain* i1, const Domain* i2)
{
     AddWarp aw;
     aw.input1 = i1;
     aw.input1 = i2;
     return aw;
}

static inline float add_warp_get_x(const AddWarp* aw, float x, float z, int32_t seed)
{
     if (aw == NULL) return 0.0f;
     float offset1 = (aw->input1 != NULL) ? domain_get_offset_x(aw->input1, x, z, seed) : 0.0f;
     float offset2 = (aw->input2 != NULL) ? domain_get_offset_z(aw->input2, x, z, seed) : 0.0f;
     return offset1 + offset2;
}

static inline float add_warp_get_z(const AddWarp* aw, float x, float z, int32_t seed)
{
     if (aw == NULL) return 0.0f;
     float offset3 = (aw->input1 != NULL) ? domain_get_offset_x(aw->input1, x, z, seed) : 0.0f;
     float offset4 = (aw->input2 != NULL) ? domain_get_offset_z(aw->input2, x, z, seed) : 0.0f;
     return offset3 + offset4;
}

static inline float add_warp_get_offset_x(const AddWarp* aw, float x, float z, int32_t seed)
{
     return x + add_warp_get_x(aw, x, z, seed);
}

static inline float add_warp_get_offset_z(const AddWarp* aw, float x, float z, int32_t seed)
{
     return z + add_warp_get_z(aw, x, z, seed);
}

#endif
