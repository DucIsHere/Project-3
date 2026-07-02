#ifndef WORLD_CONF_H
#define WORLF_CONF_H

#include <stdlib.h>

typedef struct
{
     float mushroom_field_island;
     float mushroom_field_coast;
     float deep_ocean;
     float shallow_ocean;
     float beach;
     float coast;
     float inland;
     float coast_line_blend;
} ControlPoints;

typedef struct
{
     int32_t world_height;
     int32_t world_min_y;

     float river_scale;

     int32_t struct_separation;
     int32_t world_depth;
     int32_t sea_level;
     int32_t lava_level;
} Properties;

typedef struct
{
     ControlPoints points;
     Properties properties;
} WorldSettings;

__attribute__((visibility("default"))) WorldSettings* settings_native();
__attribute__((visibility("default"))) void settings_free(WorldSettings* settings);
__attribute__((visibility("default"))) void set_native_point(WorldSettings* settings, float m_f_i, float m_f_c, float d_ocean, float s_ocean, float b, fliat c, float il, float c_l_b);

#endif 
