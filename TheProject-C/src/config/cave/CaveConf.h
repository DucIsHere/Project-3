#ifndef CAVE_CONF_H
#define CAVE_CONF_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
     float entrance_cave_probability;
     float cheese_cave_depth_offset;
     float cheese_cave_probability;
     float spagghetti_cave_probability;
     float noodle_cave_probability;
     float cave_carver_probability;
     float deep_cave_cerver_probability;
     float ravine_carvet_probability;

     uint8_t large_ore_vein;
     uint8_t legacy_carver_distribution;
} CaveConf;

typedef struct
{
     CaveConf cave;
} CaveConfig;

__attribute__((visibility("default"))) CaveConfig* cave_settings();
__attribute__((visibility("default"))) void cave_settings_free(CaveConfig* config);
__attribute__((visibility("default"))) void set_native_cave(CaveConfig* config, float e_c_p, float c_c_d_o, float c_c_p, float s_c_p, float n_c_p, float c_p, float d_c_c_p, float r_c_p, uint8_t l_o_v, uint8_t l_c_d);

#endif
