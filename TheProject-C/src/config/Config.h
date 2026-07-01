#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

typedef struct 
{
    int64_t world_height; 
    int64_t world_depth;         
    int64_t total_blocks;       

    double unit;                
    double inv_total_blocks;     

    double water_level_scaled;
    double ground_level_scaled;
    double beach_level_scaled;
    double elevation_range;
} LevelsConfig;

typedef struct
{
    uint32_t seed;
    LevelsConfig levels;
} Conf;

__attribute__((visibility("default"))) Conf init_height_64(int32_t seed, int64_t world_h, int64_t world_d, int64_t sea_l);

#endif