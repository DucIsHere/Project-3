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


typedef struct {
    int32_t continent_scale;
    float continent_jitter;
    float continent_skipping;
    float continent_size_variance;
    int32_t continent_noise_octaves;
    float continent_noise_gain;
    float continent_noise_lacunarity;
} Continent;

typedef struct
{
     ControlPoints points;
     Continent continent;
} WorldSettings;

__attribute__((visibility("default"))) WorldSettings* settings_native();
__attribute__((visibility("default"))) void settings_free(WorldSettings* settings);
__attribute__((visibility("default"))) void set_native_point(WorldSettings* settings, float m_f_i, float m_f_c, float d_ocean, float s_ocean, float b, fliat c, float il, float c_l_b);

__attribute__((visibility("default"))) void set_world_continent(WorldSettings* settings, int32_t scale, float jitter, float skipping, float variance, int32_t octaves, float gain, float lacunarity
);
#endif 
