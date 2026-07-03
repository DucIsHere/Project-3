#ifndef WORLD_CONF_C
#define WORLD_CONF_C

#include "config/WorldConf.c"
#include <stdlib.h>

WorldSettings* settings_native()
{
     WorldSettings* settings = (WorldSettings*)malloc(sizeof(WorldSettings));
     return settings;
}

void settings_free(WorlSettings* settings)
{
     if (settings)
     {
          free(settings);
     }
}

void set_native_point(WorldSettings* settings, float m_f_i, float m_f_c, float d_ocean, float s_ocean, float b, fliat c, float il, float c_l_b)
{
     if (!settings) return;
     settings->points.mushroom_field_island = m_f_i;
     settings->points.mushroom_field_coast = m_f_c;
     settings->points.deep_ocean = d_ocean;
     settings->points.shallow_ocean = s_ocean;
     settings->points.beach = b;
     settings->points.coast = c;
     settings->points.inland = il;
     settings->points.coast_line_blend = c_l_b;
}

#endif
