#ifndef LIB_CONFIG_C
#define LIB_CONFIG_C

#include "config/Config.h"

Conf init_adaptive_engine_config(int32_t seed, int32_t mapSize, int64_t max_h, int64_t min_d, int64_t sea_l)
{
    Conf cf;

    cf.levels.world_max_height = max_h;
    cf.levels.world_min_height = min_d;
    cf.levels.sea_level_block = sea_l;

    int64_t raw_range = max_h - min_d;
    cf.levels.total_world_range = (int64_t)((double)raw_range * 0.85f);

    cf.levels.block_to_unit_scaled = 1.0f / cf.levels.total_world_range;
    cf.levels.unit_to_block_scaled = (double)cf.levels.total_world_range;

}

#endif