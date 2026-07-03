#ifndef CAVE_CONF_C
#define CAVE_CONF_C

#include "config/cave/CaveConf.h"

#include <stdlib.h>

CaveConfig* cave_settings()
{
     CaveConfig* settings = (CaveConfig*)malloc(sizeof(CaveConfig));
     return settings;
}

void cave_settings_free(CaveConfig* config)
{
     if (config)
     {
          free(config);
     }
}

void set_native_cave(CaveConfig* config, float e_c_p, float c_c_d_o, float c_c_p, float s_c_p, float n_c_p, float c_p, float d_c_c_p, float r_c_p, uint8_t l_o_v, uint8_t l_c_d)
{
     if (config)
     {
          config->cave.entrance_cave_probability = e_c_p;
          config->cave.cheese_cave_depth_offset = c_c_d_o;
          config->cave.cheese_cave_probability = c_c_p;
          config->cave.spagghetti_cave_probability = s_c_p;
          config->cave.noodle_cave_probability = n_c_p;
          config->cave.cave_carver_probability = c_p;
          config->cave.deep_cave_cerver_probability = d_c_c_p;
          config->cave.ravine_carvet_probability = r_c_p;

          config->cave.large_ore_vein = l_o_v;
          config->cave.legacy_carver_distribution = l_c_d;
     }
}

#endif
