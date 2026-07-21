#ifndef GEN_WARP_H
#define GEN_WARP_H

#include <stdint.h>

#include "noise/domain/Domain.h"
#include "noise/domain/Domains.h"

typedef struct GenWarp
{
     const Domain* lake_1;
     const Domain* lake_2;
     const Domain* river_1;
     const Domain* river_2;

     uint8_t is_empty;
};

static inline GenWarp gen_warp_make(int32_t seed, const Noise* n_lake1_x, const Noise* n_lake1_z,
                                                  const Noise* n_lake2_x, const Noise* n_lake2_z,
                                                  const Noise* n_river1_x, const Noise* n_river1_z,
                                                  const Noise* n_river2_x, const Noise* n_river2_z)
{
     GenWarp gw;
     gw.is_empty = 0;

     gw.lake_1 = domain_create(n_lake1_x, n_lake1_z, 200.0f);
     gw.lake_2 = domain_create(n_lake2_x, n_lake2_z, 50.0f);

     gw.river_1 = domain_create(n_river1_x, n_river1_z, 95.0f);
     gw.river_2 = domain_create(n_river2_x, n_river2_z, 16.0f);

     return gw;
}

static inline GenWarp gen_warp_empty(void)
{
     GenWarp gw;
     gw.is_empty = 1;

     gw.lake_1 = domains_direct();
     gw.lake_2 = domains_direct();
     gw.river_1 = domains_direct();
     gw.river_2 = domains_direct();

     return gw;
}

/**
 * Tính toán Tọa độ X đã biến dạng (Warped X) cho Lake
 */
static inline float gen_warp_get_lake_x(const GenWarp* warp, float x, float z, int32_t seed)
{
    if (warp->is_empty) return x;
    
    // Cộng dồn offset của 2 tầng lake (mô phỏng Domains.add)
    float offset = domain_get_offset_x(&warp->lake_1, x, z, seed) + 
                   domain_get_offset_x(&warp->lake_2, x, z, seed);
    return x + offset;
}

/**
 * Tính toán Tọa độ Z đã biến dạng (Warped Z) cho Lake
 */
static inline float gen_warp_get_lake_z(const GenWarp* warp, float x, float z, int32_t seed)
{
    if (warp->is_empty) return z;

    float offset = domain_get_offset_z(&warp->lake_1, x, z, seed) + 
                   domain_get_offset_z(&warp->lake_2, x, z, seed);
    return z + offset;
}

/**
 * Tính toán Tọa độ X đã biến dạng (Warped X) cho River
 */
static inline float gen_warp_get_river_x(const GenWarp* warp, float x, float z, int32_t seed)
{
    if (warp->is_empty) return x;

    float offset = domain_get_offset_x(&warp->river_1, x, z, seed) + 
                   domain_get_offset_x(&warp->river_2, x, z, seed);
    return x + offset;
}

/**
 * Tính toán Tọa độ Z đã biến dạng (Warped Z) cho River
 */
static inline float gen_warp_get_river_z(const GenWarp* warp, float x, float z, int32_t seed)
{
    if (warp->is_empty) return z;

    float offset = domain_get_offset_z(&warp->river_1, x, z, seed) + 
                   domain_get_offset_z(&warp->river_2, x, z, seed);
    return z + offset;
}

#endif
