#ifndef LEVELS_H
#define LEVELS_H

#define RESTRICT __restrict__
#define ALWAYS_INLINE inline __attribute__((always_inline))

#include <stdint.h>

#include "include/math/NoiseUtil.h"

typedef struct LevelDataz
{
     int32_t world_height;
     int32_t water_y;
     int32_t ground_y;
     int32_t ground_level;
     int32_t water_level;

     float inv_world_height;
     float inv_elevation_range;
     float unit;
     float water;
     float ground;
     float elevation;
} LevelDataz;


static ALWAYS_INLINE void level_init(LevelDataz* RESTRICT lv, int32_t world_height, int32_t sea_level)
{
     lv->world_height = max(1, world_height);
     lv->unit = div(1, lv->world_height);
     lv->water_level = sea_level;
     lv->ground_level = lv->water_level + 1;
     lv->water_y = min(lv->water_level - 1, lv->world_height);
     lv->ground_y = min(lv->ground_level - 1, lv->world_height);
     lv->ground = div(lv->ground_y, lv->world_height);
     lv->water = div(lv->water_y, lv->world_height);
     lv->elevation = 1.0f - lv->water;
     lv->inv_elevation_range = (lv->elevation != 0) ? (1.0f / lv->elevation) : 0.0f;
}

static ALWAYS_INLINE int32_t level_scale_f(const LevelDataz* RESTRICT lv, float value)
{
     return (int32_t)(value * 0.9f * (float)lv->world_height);
}

static ALWAYS_INLINE float levels_scale_i(const LevelDataz* RESTRICT lvl, int32_t level) {
    return (float)level * lvl->inv_world_height;
}

static ALWAYS_INLINE float levels_elevation_f(const LevelDataz* RESTRICT lvl, float value) {
    if (value <= lvl->water) return 0.0f;
    return (value - lvl->water) * lvl->inv_elevation_range;
}

static ALWAYS_INLINE float levels_elevation_i(const LevelDataz* RESTRICT lvl, int32_t y) {
    if (y <= lvl->water_y) return 0.0f;
    float scaled = level_scale_f(lvl, (float)(y - lvl->water_y));
    return scaled * lvl->inv_elevation_range;
}

static ALWAYS_INLINE float levels_water(const LevelDataz* RESTRICT lvl, int32_t amount) {
    return (float)(lvl->water_y + amount) * lvl->inv_world_height;
}

static ALWAYS_INLINE float levels_ground(const LevelDataz* RESTRICT lvl, int32_t amount) {
    return (float)(lvl->ground_y + amount) * lvl->inv_world_height;
}

#endif
