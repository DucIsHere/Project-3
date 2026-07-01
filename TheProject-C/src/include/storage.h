#ifndef STORAGE_H
#define STORAGE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdatomic.h>
#include <stddef.h>

typedef struct __attribute__((aligned(64)))
{
    size_t tile_width;
    size_t tile_length;
    size_t total_cell;
    size_t total_terrai;

    _Atomic float* cell_height;
    _Atomic float* cell_sediment;
    _Atomic float* cell_gradient;
    _Atomic float* cell_erosion;
    _Atomic float* cell_temperature;
    _Atomic float* cell_moisture;
    _Atomic float* cell_heightErosion;
    _Atomic float* cell_regionMoisture;
    _Atomic float* cell_regionTemperature;
    _Atomic float* cell_continentId;
    _Atomic float* cell_continentEdge;
    _Atomic float* cell_continentDistances;
    _Atomic float* cell_terrainRegionId;
    _Atomic float* cell_terrainRegionEdge;
    _Atomic float* cell_biomeRegionId;
    _Atomic float* cell_biomeRegionEdge;
    _Atomic float* cell_macroBiomeId;
    _Atomic float* cell_riverMask;
    _Atomic float* cell_weirdness;
    _Atomic float* cell_beachNoises;

    _Atomic uint32_t cell_continentXs;
    _Atomic uint32_t cell_continentZs;

    _Atomic uint8_t cell_erosionMask;
    _Atomic uint8_t cell_terrainIds;
    _Atomic uint8_t cell_biomeIds;

    _Atomic uint8_t terrain_none;
    _Atomic uint8_t terrain_deepOcean;
    _Atomic uint8_t terrain_ocean;

    float* velocityX;
    float* velocityZ;

    float* iceMap;
    float* sedimentMap;
    float* fluxN;
    float* fluxS;
    float* fluxE;
    float* fluxW;

    float* fluxMap;

    float* deltaSediment;

    float* waterFlux;
    float* thermalFlux;
    float* soluteFlux;
    float* flowAccumulation;
    float* currentThermal;

    float* collapseBuffer;
    float* sedimentBuffer;

    float* damage;
    float* moisture;
    float* temperature;
    float* sediment;

    float* h;
    float* s;
    float* tau0;

    float* velocityMap;
    int* snowDepth;
} TileSoA

__attribute__((visibility("default"))) TileSoA* system_allocate_tile(size_t length, size_t width);
__attribute__((visibility("default"))) void system_free_tile(TileSoA* tile);

#endif