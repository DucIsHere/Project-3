#ifndef TERRAIN_CONF_H
#define TERRAIN_CONF_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	int32_t terrain_seed_offset;
	int32_t terrain_region_size;

	float global_vertical_scale;
	float global_horizontal_scale;

	bool fancy_mountains;
} TerrainGeneral;

typedef struct
{
	float weight;
	float base_scale;
	float vertical_scale;
	float horizontal_scale;
	float base_height;
	float valley_depth;
	float valley_width;
	float mountain_height_scale;
	float mountain_scale;
	float plateau_height;
	float coast_sharpness;
	float valley_erosion;
	float valley_weirdness;
	float river_erosion;
	float river_weirdness;
	float lake_weirdness;
	float beach_noise_scale;
	float beach_height;

	int32_t aquifer_depth_offset;
} TerrainTypr;

typedef struct
{
	TerrainGeneral General;

	TerrainTypr steppe;
	TerrainTypr plains;
	TerrainTypr hills;
	TerrainTypr dales;
	TerrainTypr plateau;
	TerrainTypr badlands;
	TerrainTypr torridonian;
	TerrainTypr mountains;
	TerrainTypr volcano;
	TerrainTypr blend_low;
	TerrainTypr blend_mid;
	TerrainTypr blend_high;
} TerrainSettings;

__attribute__((visibility("default"))) TerrainSettings* native_settings();
__attribute__((visibility("default"))) void free_settungs(TerrainSettings* settings);
__attribute__((visibility("default"))) void set_native_general(FullTerrainSettings* settings, int32_t offset, int32_t size, float v_scale, float h_scale, bool fancy);
__attribute__((visibility("default"))) void set_native_biome(FullTerrainSettings* settings, int32_t biome_id, float weight, float b_scale, float v_scale, float h_scale, float b_height, float s_scale, float v_depth, float v_width, float m_h_scale, float m_scale, float p_height, float c_sharp, float v_erosion, float v_weirdness, float r_erosion, float r_weirdness, float l_weirdness, float b_n_scale, float b_height, int32_t aq_offset);

#endif
