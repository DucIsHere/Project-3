#ifndef CONTINENT_LERP_2
#define CONTINENT_LERP_2

#include <stdint.h>

#include "include/math/NoiseUtil.h"
#include "cell/Cell.h"
#include "cell/CellPopulator.h"
#include "noise/function/Interpolation.h"

typedef struct
{
     PopulateFunc lower;
     PopulateFunc upper;
     InterpolationType interpolation;

     float blend_lower;
     float blend_upper;
     float blend_range;
} Continent2;

#endif
