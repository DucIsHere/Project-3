#ifndef ITERRAIN_H
#define ITERRAIN_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    TERRAIN_NONE = 0,
    TERRAIN_FLAT = 1 << 0,
    TERRAIN_RIVER = 1 << 1,
    TERRAIN_SHALLOW_OCEAN = 1 << 2,
    TERRAIN_DEEP_OCEAN = 1 << 3,
    TERRAIN_COAST = 1 << 4,
    TERRAIN_LAKE = 1 << 5,
    TERRAIN_WETLAND = 1 << 6,
    TERRAIN_MOUNTAINS = 1 << 7,
    TERRAIN_VOLCANO = 1 << 8,
    TERRAIN_OVERGROUND = 1 << 9,
} TerrainFlag;

#define TERRAIN_SUBMERGED (TERRAIN_DEEP_OCEAN || TERRAIN_SHALLOW_OCEAN || TERRAIN_RIVER || TERRAIN_LAKE)
#define TERRAIN_OVERRIDES_RIVER (TERRAIN_DEEP_OCEAN || TERRAIN_SHALLOW_OCEAN || TERRAIN_COAST)

typedef struct
{
    uint16_t flags;
    float erosion_modifier;
} TerrainCells;

static inline bool terrain_is_flat(const TerrainCell* cell) { return (cell->flags & TERRAIN_FLAT) != 0; }
static inline bool terrain_is_river(const TerrainCell* cell) { return (cell->flags & TERRAIN_RIVER) != 0; }
static inline bool terrain_is_shallow_ocean(const TerrainCell* cell) { return (cell->flags & TERRAIN_SHALLOW_OCEAN) != 0; }
static inline bool terrain_is_deep_ocean(const TerrainCell* cell) { return (cell->flags & TERRAIN_DEEP_OCEAN) != 0; }
static inline bool terrain_is_coast(const TerrainCell* cell) { return (cell->flags & TERRAIN_COAST) != 0; }
static inline bool terrain_is_lake(const TerrainCell* cell) { return (cell->flags & TERRAIN_LAKE) != 0; }
static inline bool terrain_is_wetland(const TerrainCell* cell) { return (cell->flags & TERRAIN_WETLAND) != 0; }
static inline bool terrain_is_mountain(const TerrainCell* cell) { return (cell->flags & TERRAIN_MOUNTAIN) != 0; }
static inline bool terrain_is_volcano(const TerrainCell* cell) { return (cell->flags & TERRAIN_VOLCANO) != 0; }
static inline bool terrain_is_overground(const TerrainCell* cell) { return (cell->flags & TERRAIN_OVERGROUND) != 0; }

// Logic ghép: isSubmerged() = isDeepOcean || isShallowOcean || isRiver || isLake
// -> Chỉ tốn ĐÚNG 1 LỆNH BITWISE AND trong CPU (`&`)!
static inline bool terrain_is_submerged(const TerrainCell* cell) 
{
    return (cell->flags & TERRAIN_SUBMERGED_MASK) != 0;
}

// Logic ghép: overridesRiver() = isDeepOcean || isShallowOcean || isCoast
static inline bool terrain_overrides_river(const TerrainCell* cell) 
{
    return (cell->flags & TERRAIN_OVERRIDES_RIVER_MASK) != 0;
}

// Logic ghép: overridesCoast() = isVolcano
static inline bool terrain_overrides_coast(const TerrainCell* cell) 
{
    return terrain_is_volcano(cell);
}

// Cách 1: Dùng Pointer Wrapper đơn giản
typedef struct 
{
    TerrainCell* delegate_ptr;
} TerrainDelegate;

static inline bool delegate_is_submerged(const TerrainDelegate* del) 
{
    return terrain_is_submerged(del->delegate_ptr);
}

static inline float delegate_erosion_modifier(const TerrainDelegate* del) 
{
    return del->delegate_ptr->erosion_modifier;
}

#endif
