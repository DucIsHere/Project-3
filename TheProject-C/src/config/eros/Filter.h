#pragma once
#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

typedef struct
{
	int32_t droplets_per_chunk;
	int32_t droplet_life_time;

	float droplet_volume;
	float droplet_velocity;
	float erosion_rate;
	float deposite_rate;
	
	float fluvial_stream_power_k;
	float fluvial_flux_exponent;
	float fluvial_slope_exponent;
	float min_fluvial_erosion;
	float max_fluvial_erosion;
	float lateral_erosion_factor;
} HydraulicEros;

typedef struct
{
	float abrastion_rate;
	float deposit_speed;
	float accumulation_rate;
	float melt_rate;
	float viscocity;
	float plucking_probability;
	float plucking_rate;
	float initial_speed;
	float initial_ice_volume;

	int32_t max_life_time;
} GlacialEros;

typedef struct
{
	float flow_rate;
	float max_sediment_flow;
	float weathering_rate;
	float lobe_frictionl;
} Soilfluction;

typedef struct
{
	float base_viscosity;
	float solubility;
	float river_threshold;
	float flow_momentum;
	float infiltration_rate;
} SubsurfaceFlow;

typedef struct
{
	float base_porosity;
	float permeability_scale;
	float recharge_rate;
	float pressure_factor;
	float simulate_flow;
} Aquifer;

typedef struct
{
	float erode_strength;
	float deposit_strength;
	float min_height;
} AeroEros;

typedef struct
{
	float base_water_level;
	float erosion_scale;
	float abrasion_factor;
	float tide_amplitude;
	float tide_frequency;
	float critical_angle;
} CoastalEros;

typedef struct
{
	float talus_threshold;
	float material_transfer;
} ThermalEros;

typedef struct
{
	float smoothing_rate;
	float smoothing_radius;
} Smoothing;

typedef struct
{
	HydraulicEros Hydrau;
	GlacialEros Glacial;
	Soilfluction Soil;
	SubsurfaceFlow Subsurface;
	Aquifer Aquifer;
	AeroEros Aeolian;
	CoastalEros Coastal;
	ThermalEros Thermal;
	Smoothing Smoothing;
} FilterSetings;

#endif 
