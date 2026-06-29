#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

typedef struct 
{
    int64_t world_max_height;
    int64_t world_min_height;
    int64_t total_world_range;
    int64_t sea_level_block;

    double block_to_unit_scale;
    double unit_to_block_scale;

    float water_level_scaled;
    float ground_level_scaled;
    float elevation_range;
} LevelsConfig;

typedef struct
{
    float global_sediment;
    float global_temperature_offset;
    float global_moisture_offset;
    float river_frequency;
    float beach_line_noise;
} EnviromentConfig;

typedef struct
{
    uint32_t seed;
    uint32_t mapSize;
    LevelsConfig levels;
    EnviromentConfig env;
} Conf;

__attribute__((visibility("default"))) Conf init_adaptive_engine_config(int32_t seed, int32_t mapSize, int64_t max_h, int64_t min_d, int64_t sea_l);

#endif