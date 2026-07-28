#ifndef TERRAIN_CATEGORY_H
#define TERRAIN_CATEGORY_H

#include <stdbool.h>

#include "ITerrain.h"

#ifndef TERRAIN_SUBMERGED_MASK
#define TERRAIN_SUBMERGED_MASK (TERRAIN_DEEP_OCEAN | TERRAIN_SHALLOW_OCEAN | TERRAIN_RIVER | TERRAIN_LAKE)
#endif

#ifndef TERRAIN_OVERRIDES_RIVER_MASK
#define TERRAIN_OVERRIDES_RIVER_MASK (TERRAIN_DEEP_OCEAN | TERRAIN_SHALLOW_OCEAN | TERRAIN_COAST)
#endif

typedef enum
{
    NONE = 0,
    DEEP_OCEAN = 1 << 0,
    SHALLOW_OCEAN = 1 << 1,
    COAST = 1 << 2,
    BEACH = 1 << 3,
    RIVER = 1 << 4,
    LAKE = 1 << 5, 
    WETLAND = 1 << 6,
    FLAT_LAND = 1 << 7,
    LOW_LAND = 1 << 8,
    HIGH_LAND = 1 << 9,
    COUNT = 1 << 10
} TerrainCategoryFlag;

static const uint32_t TERRAIN_CATEGORY_FLAG[COUNT]
{
    [NONE] = TERRAIN_NONE,
    [DEEP_OCEAN] = TERRAIN_DEEP_OCEAN, 
    [SHALLOW_OCEAN] = TERRAIN_SHALLOW_OCEAN,
    [COAST] = TERRAIN_COAST | TERRAIN_OVERGROUND,
    [BEACH] = TERRAIN_OVERGROUND | TERRAIN_COAST,
    [RIVER] = TERRAIN_RIVER,
    [LAKE] = TERRAIN_LAKE,
    [WETLAND] = TERRAIN_WETLAND | TERRAIN_OVERGROUND,
    [FLAT_LAND] = TERRAIN_FLAT | TERRAIN_OVERGROUND,
    [LOW_LAND] = TERRAIN_OVERGROUND,
    [HIGH_LAND] = TERRAIN_OVERGROUND

}

static inline uint32_t terrain_category_get_flags(TerrainCategoryFlag flag)
{
    if (flag < 0 || flag > COUNT) return TERRAIN_NONE;
    return TERRAIN_CATEGORY_FLAG[flag];
}

static inline bool terrain_category_is_flat(TerrainCategoryFlag* flag)
{
    return (terrain_category_get_flags(flag) & TERRAIN_FLAT) != 0;
}

static inline bool terrain_category_is_river(TerrainCategoryFlag* flag)
{
    return (terrain_category_get_flags(flag) & TERRAIN_RIVER) != 0;
}

static inline bool terrain_category_is_lake(TerrainCategoryFlag* flag)
{
    return (terrain_category_get_flags(flag) & TERRAIN_LAKE) != 0;
}

static inline bool terrain_category_is_deep_ocean(TerrainCategoryFlag* flag)
{
    return (terrain_category_get_flags(flag) & TERRAIN_DEEP_OCEAN) != 0;
}

static inline bool terrain_category_is_shallow_ocean(TerrainCategoryFlag* flag)
{
    return (terrain_category_get_flags(flag) & TERRAIN_SHALLOW_OCEAN) != 0;
}

static inline bool terrain_category_is_submerged(TerrainCategoryFlag* flag)
{
    return (terrain_category_get_flags(flag) & TERRAIN_SUBMERGED_MASK) != 0;
}

static inline bool terrain_categoty_is_overrides_river(TerrainCategoryFlag* flag)
{
    return (terrain_category_get_flags(flag) & TERRAIN_OVERRIDES_RIVER_MASK) != 0;
}

static inline bool terrain_category_is_coast(TerrainCategoryFlag* flag)
{
    return (terrain_category_get_flags(flag) & TERRAIN_COAST) != 0;
}

static inline bool terrain_category_is_wetland(TerrainCategoryFlag* flag)
{
    return (terrain_category_flag(flags) & TERRAIN_WETLAND) != 0;
}

static inline bool terrain_category_is_overground(TerrainCategotyFlag* flags)
{
    return (terrain_category_get_flags(flags) & TERRAIN_OVERGROUND) != 0;
}

static inline bool terrain_category_is_low_land(TerrainCategoryFlag* flag) { return flag == LOW_LAND }
static inline bool terrain_category_is_high_land(TerrainCategoryFlag* flag) { return flag == HIGH_LAND }
static inline bool terraon_category_is_beach(TerrainCategoryFlag* flag) { return flag = BEACH }

static inline TerraiCategoryFlag terrain_category_get_dominant(TerrainCategoryFlag* flag)
{
    return (cat1 > cat2) ? cat1 : cat2;
}

#endif
