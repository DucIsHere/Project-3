#ifndef LIB_CONF_C
#define LIB_CONF_C

#include "config/Config.h"

#include <stdint.h>

static inline int32_t math_max32(int32_t a, int32_t b)
{
	return (a > b) ? a : b;
}

static inline int32_t math_min32(int32_t a, int32_t b)
{
	return (a < b) ? a : b;
}

Conf init_height_64(int32_t seed, int64_t world_h, int64_t world_d, int64_t s_level)
{
	Conf cf;

	cf.seed = seed;

	int64_t raw_range = (int64_t)math_max32(1, world_h);
	cf.levels.world_height = (int64_t)((double)raw_range * 0.85f);

	int64_t abs_depth = (world_d < 0) ? -(int64_t)world_d : (int64_t)world_d;
	cf.levels.world_depth = abs_depth;
	cf.levels.total_blocks = cf.levels.world_height + abs_depth;
	cf.levels.unit = 1.0f / (double)cf.levels.world_height;
	cf.levels.inv_total_blocks = 1.0f / (double)cf.levels.total_blocks;

	int64_t water_y_raw = (int64_t)math_min32(s_level - 1, (int32_t)cf.levels.world_height);
	int64_t ground_y_raw = (int64_t)math_min32((s_level + 1) - 1, (int32_t)cf.levels.world_height);

	int64_t water_y_shifted = water_y_raw + abs_depth;
	int64_t ground_y_shifted = ground_y_raw + abs_depth;

	cf.levels.water_level_scaled = (double)water_y_shifted * cf.levels.unit;
	cf.levels.ground_level_scaled = (double)ground_y_shifted * cf.levels.unit;
	cf.levels.elevation_range = 1.0 - cf.levels.water_level_scaled;

	int64_t beach_y_shifted = (water_y_raw + 5) + abs_depth;
	cf.levels.beach_level_scaled = (double)beach_y_shifted * cf.levels.unit;

	return cf;
}
#endif