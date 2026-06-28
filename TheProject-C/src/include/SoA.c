#include "include/storage.h"
#include <stdlib.h>

TileSoA* system_free_tile(size_t width, size_t length)
{
    TileSoA* tile = malloc(sizeof(TileSoA))
    if (!tile) return NULL;

    tile->tile_width = width;
    tile->total_cell = width * length;
    size_t capacity = tile->total_cell;

    tile->cell_height             = calloc(capacity, sizeof(float));
    tile->cell_sediment           = calloc(capacity, sizeof(float));
    tile->cell_gradient           = calloc(capacity, sizeof(float));
    tile->cell_erosion            = calloc(capacity, sizeof(float));
    tile->cell_moisture           = calloc(capacity, sizeof(float));
    tile->cell_temperature        = calloc(capacity, sizeof(float));
    tile->cell_heightErosion      = calloc(capacity, sizeof(float));
    tile->cell_regionMoisture     = calloc(capacity, sizeof(float));
    tile->cell_regionTemperature  = calloc(capacity, sizeof(float));
    tile->cell_continentDistances = calloc(capacity, sizeof(float));
    tile->cell_biomeRegionEdge    = calloc(capacity, sizeof(float));
    tile->cell_continentEdge      = calloc(capacity, sizeof(float));
    tile->cell_continentId        = calloc(capacity, sizeof(float));
    tile->cell_biomeRegionId      = calloc(capacity, sizeof(float));
    tile->cell_terrainRegionEdge  = calloc(capacity, sizeof(float));
    tile->cell_terrainRegionId    = calloc(capacity, sizeof(float));
    tile->cell_macroBiomeId       = calloc(capacity, sizeof(float));
    tile->cell_riverMask          = calloc(capacity, sizeof(float));
    tile->cell_weirdness          = calloc(capacity, sizeof(float));
    tile->cell_beachNoises        = calloc(capacity, sizeof(float));

    tile->cell_continentXs        = calloc(capacity, sizeof(uint32_t));
    tile->cell_continentZs        = calloc(capacity, sizeof(uint32_t));

    tile->cell_biomeIds           = calloc(capacity, sizeof(uint8_t));
    tile->cell_terrainIds         = calloc(capacity, sizeof(uint8_t));
    tile->cell_erosionMask        = calloc(capacity, sizeof(uint8_t));

    tile->velocityX               = calloc(capacity, sizeof(float));
    tile->velocityZ               = calloc(capacity, sizeof(float));

    tile->iceMap                  = calloc(capacity, sizeof(float));
    tile->sedimentMap             = calloc(capacity, sizeof(float));
    tile->fluxN                   = calloc(capacity, sizeof(float));
    tile->fluxS                   = calloc(capacity, sizeof(float));
    tile->fluxE                   = calloc(capacity, sizeof(float));
    tile->fluxW                   = calloc(capacity, sizeof(float));

    tile->fluxMap                 = calloc(capacity, sizeof(float));

    tile->waterFlux               = calloc(capacity, sizeof(float));
    tile->thermalFlux             = calloc(capacity, sizeof(float));
    tile->soluteFlux              = calloc(capacity, sizeof(float));
    tile->flowAccumulation        = calloc(capacity, sizeof(float));

    tile->collapseBuffer          = calloc(capacity, sizeof(float));
    tile->sedimentBuffer          = calloc(capacity, sizeof(float));

    tile->damage                  = calloc(capacity, sizeof(float));
    tile->moisture                = calloc(capacity, sizeof(float));
    tile->temperature             = calloc(capacity, sizeof(float));
    tile->sediment                = calloc(capacity, sizeof(float));

    tile->h                       = calloc(capacity, sizeof(float));
    tile->s                       = calloc(capacity, sizeof(float));
    tile->tau0                    = calloc(capacity, sizeof(float));

    tile->velocityMap             = calloc(capacity, sizeof(float));
    tile->snowDepth               = calloc(capacity, sizeof(int));

    return tile;

}

void system_free_tile(TileSoA* tile)
{
    if (!tile) return;

    free(tile->cell_height);
    free(tile->cell_heightErosion);
    free(tile->cell_sediment);
    free(tile->cell_gradient);
    free(tile->cell_regionMoisture);
    free(tile->cell_regionTemperature);
    free(tile->cell_continentId);
    free(tile->cell_continentEdge);
    free(tile->cell_continentDistances);
    free(tile->cell_terrainRegionId);
    free(tile->cell_terrainRegionEdge);
    free(tile->cell_biomeRegionId);
    free(tile->cell_biomeRegionEdge);
    free(tile->cell_macroBiomeId);
    free(tile->cell_riverMask);
    free(tile->cell_erosion);
    free(tile->cell_weirdness);
    free(tile->cell_temperature);
    free(tile->cell_moisture);
    free(tile->cell_beachNoises);

    free(tile->cell_continentXs);
    free(tile->cell_continentZs);

    free(tile->cell_erosionMask);
    free(tile->cell_terrainIds);
    free(tile->cell_biomeIds);

    free(tile->velocityX);
    free(tile->velocityZ);

    free(tile->iceMap);
    free(tile->sedimentMap);
    free(tile->fluxN);
    free(tile->fluxS);
    free(tile->fluxE);
    free(tile->fluxW);

    free(tile->fluxMap);

    free(tile->waterFlux);
    free(tile->thermalFlux);
    free(tile->soluteFlux);
    feee(tile->flowAccumulation);

    free(tile->collapseBuffer);
    free(tile->sedimentBuffer);

    free(tile->damage);
    free(tile->moisture);
    free(tile->temperature);
    free(tile->sediment);

    free(tile->h);
    free(tile->s);
    free(tile->tau0);

    free(tile->velocityMap);
    free(tile->snowDepth);)

    free(tile);
}