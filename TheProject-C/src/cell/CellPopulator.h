#ifndef CELL_POPULATOR_H
#define CELL_POPULATOR_H

#include <stdint.h>
#include "Cell.h"

typedef void (*PopulateFunc)(
     float* RESTRICT height,
     float* RESTRICT river_map,
     uint8_t* RESTRICT terrain,
     const float* RESTRICT xs,
     const float* RESTRICT zs,
     int count
);

static inline void cell_populate_chunkx16(
     CellSoA* RESTRICT soa,
     const float* RESTRICT xs,
     const float* RESTRICT zs
)
{
     float* RESTRICT height = soa->height;
     float* RESTRICT river_mask = soa->river_mask;
     uint8_t RESTRICT terrain = soa->terrain;

     #if defined(__GNUC__) || defined(__clang__)
         #pragma clang loop vectorize(enable) interleave(enable)
     #endif
}

#endif
