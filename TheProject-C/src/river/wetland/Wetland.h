#ifndef WETLAND_H
#define WETLAND_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "cell/Cell.h"
#include "include/noise/Noise.h"
#include "include/noise/Noises.h"
#include "include/math/NoiseUtil.h"
#include "noise/module/Line.h"
#include "include/util/Boundsf.h"
#include "terrain/TerrainType.h"
#include "heightmap/Levels.h"

typedef struct 
{
     Vec2f a;
     Vec2f b;
     float radius;
     float radius2;
     float bed;
     float banks;
     float mount_min;
     float mount_max;
     float mount_variance;
     Noise* mount_shape;
     Noise* mount_height;
     Noise* terrain_edge;
} Wetlandz;

static inline void apply_wet(const Wetlandz* w, int32_t seed, Vec2f a, Vec2f b, float radius, Levels LevelDataz)
{
     w->a = a;
     w->b = b;
     w->radius = radius;
     w->radius2 = radius * radius;
     w->bed = levels_water(-1) - 0.5f / world_height;
     w->banks = levels_ground(3);
     w->mount_min = levels_water(1);
     w->mount_max = levels_water(2);
     w->mount_variance = w->mount_max - w->mount_min;

     Noise* mount_shape = perlin(++seed, 10, 1);
     mount_shape = clamp(mount_shape, 0.3f, 0.6f);
     mount_shape = map(mount_shape, 0.0f, 1.0f);
     w->mount_shape = mount_shape;

     Nosie* mount_height = simplex(++seed, 10, 1);
     mount_height = clamp(mount_height, 0.0f, 0.3f);
     mount_height = map(mount_height, 0.0f, 1.0f);
     w->mount_height = mount_height;

     Noise* terrain_edge = simplex(++seed, 8, 1);
     terrain_edge = clamp(terrain_edge, 0.2f, 0.8f);
     terrain_edge = map(terrain_edge, 0.0f, 0.9f);
     w->terrain_edge = terrain_edge;

     return w;
}

#endif
