#ifndef TERRAIN_CONF_C
#define TERRAIN_CONF_C

#include "config/TerrainConf.h"
#include <stdlib.h>

TerrainSettings* native_settings()
{
     TerrainSettings* settings = (TerrainSettings*)malloc(sizeof(TerrainSettings));
     return settings;
}

void free_settings(TerrainSettings* settings)
{
     if (settings)
     {
          free(settings);
     }
}

void set_native_general(TerrainSettings* settings, int32_t offset, int32_t size, float v_scale, float h_scale, bool fancy)
{
     settings->general.terrain_seed_offset = offset;
     settings->general.terrain_region_size = size;
     settings->general.global_vertical_scale = v_scale;
     settings->general.global_horizontal_scale = h_scale;
     settings->general.fancy_mountains = fancy;
}

void set_native_biome(TerrainSettings* settings, int32_t biome_id, float weight, float b_scale, float v_scale, float h_scale, float b_height, float v_depth, float v_width, float m_h_scale, float m_scale, float p_height, float c_sharp, float v_erosion, float v_weirdness, float r_erosion, float r_weirdness, float l_weirdness, float b_n_scale, float beach_h)
{
     if (!settings) return;

     // Con trỏ trung gian đến sub-struct trong TerrainSettings và TerrainTypr
     TerrainTypr* target = NULL;
     
     switch (biome_id)
     {
          case 0:
               target = &settings->steppe;
               break;
          case 1:
               target = &settings->plains;
               break;
          case 2:
               target = &settings->hills;
               break;
          case 3:
               target = &settings->dales;
               break;
          case 4:
               target = &settings->plateau;
               break;
          case 5:
               target = &settings->badlands;
               break;
          case 6:
               target = &settings->torridonian;
               break;
          case 7:
               target = &settings->mountains;
               break;
          case 8:
               target = &settings->volcano;
               break;
          case 9:
               target = &settings->blend_low;
               break;
          case 10:
               target = &settings->blend_mid;
               break;
          case 11: 
               target = &settings->blend_high;
               break;
          default: 
               return;
     }

     if (target)
     {
         target->weight = weight;
         target->base_scale = b_scale;
         target->vertical_scale = v_scale;
         target->horizontal_scale = h_scale;
         target->base_height = b_height;
         target->valley_depth = v_depth;
         target->valley_width = v_width;
         target->mountain_height_scale = m_h_scale;
         target->mountain_scale = m_scale;
         target->plateau_height = p_height;
         target->coast_sharpness = c_sharp;
         target->valley_erosion = v_erosion;
         target->valley_weirdness = v_weirdness;
         target->river_erosion = r_erosion;
         target->river_weirdness = r_weirdness;
         target->lake_weirdness = l_weirdness;
         target->beach_noise_scale = b_n_scale;
         target->beach_height = beach_h;
     }

}

#endif
