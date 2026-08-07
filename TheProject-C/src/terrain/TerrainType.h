#ifndef TERRAIN_TYPE_H
#define TERRAIN_TYPE_H

#include <stdint.h>
#include "ITerrain.h"

typedef enum
{
     NONE = 0,
     DEEP_OCEAN = 1,
     SHALLOW_OCEAN = 2,
     BEACh = 3,
     COAST = 4,
     RIVER = 5,
     LAKE = 6,
     WETLAND = 7,
     FLATS = 8,
     BADLANDS = 9, 
     PLATEU = 10,
     HILLS = 11,
     MOUNTAINS_1 = 12,
     MOUNTAINS_2 = 13,
     MOUNTAINS_3 = 14,
     MOUNTAINS_CHAIN = 15,
     VOLCANO = 16,
     VOLCANO_PIPE = 17
} TerrainType;

#endif
