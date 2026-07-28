#ifndef ITERRAIN_H
#define ITERRAIN_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    TERRAIN_NONE           = 0,
    TERRAIN_FLAT           = 1 << 0,
    TERRAIN_RIVER          = 1 << 1,
    TERRAIN_SHALLOW_OCEAN  = 1 << 2,
    TERRAIN_DEEP_OCEAN     = 1 << 3,
    TERRAIN_COAST          = 1 << 4,
    TERRAIN_LAKE           = 1 << 5,
    TERRAIN_WETLAND        = 1 << 6,
    TERRAIN_MOUNTAINS      = 1 << 7,
    TERRAIN_VOLCANO        = 1 << 8,
    TERRAIN_OVERGROUND     = 1 << 9
} TerrainFlag;

#define TERRAIN_SUBMERGED_MASK (TERRAIN_DEEP_OCEAN | TERRAIN_SHALLOW_OCEAN | TERRAIN_RIVER | TERRAIN_LAKE)
#define TERRAIN_OVERRIDES_RIVER_MASK (TERRAIN_DEEP_OCEAN | TERRAIN_SHALLOW_OCEAN | TERRAIN_COAST)

typedef struct
{
    uint32_t flags;
    float erosion_modifier;
} TerrainCell;

static inline TerrainCell terrain_cell_create(uint32_t flags, float erosion_modifier) 
{
    TerrainCell cell;
    cell.flags = flags;
    cell.erosion_modifier = erosion_modifier;
    return cell;
}

// ITerrain default methods
static inline float terrain_erosion_modifier(const TerrainCell* cell) 
{ 
    return cell ? cell->erosion_modifier : 1.0f; 
}

static inline bool terrain_is_flat(const TerrainCell* cell) { return cell && (cell->flags & TERRAIN_FLAT) != 0; }
static inline bool terrain_is_river(const TerrainCell* cell) { return cell && (cell->flags & TERRAIN_RIVER) != 0; }
static inline bool terrain_is_shallow_ocean(const TerrainCell* cell) { return cell && (cell->flags & TERRAIN_SHALLOW_OCEAN) != 0; }
static inline bool terrain_is_deep_ocean(const TerrainCell* cell) { return cell && (cell->flags & TERRAIN_DEEP_OCEAN) != 0; }
static inline bool terrain_is_coast(const TerrainCell* cell) { return cell && (cell->flags & TERRAIN_COAST) != 0; }
static inline bool terrain_is_lake(const TerrainCell* cell) { return cell && (cell->flags & TERRAIN_LAKE) != 0; }
static inline bool terrain_is_wetland(const TerrainCell* cell) { return cell && (cell->flags & TERRAIN_WETLAND) != 0; }
static inline bool terrain_is_mountain(const TerrainCell* cell) { return cell && (cell->flags & TERRAIN_MOUNTAINS) != 0; }
static inline bool terrain_is_volcano(const TerrainCell* cell) { return cell && (cell->flags & TERRAIN_VOLCANO) != 0; }
static inline bool terrain_is_overground(const TerrainCell* cell) { return cell && (cell->flags & TERRAIN_OVERGROUND) != 0; }

static inline bool terrain_is_submerged(const TerrainCell* cell) 
{
    return cell && (cell->flags & TERRAIN_SUBMERGED_MASK) != 0;
}

static inline bool terrain_overrides_river(const TerrainCell* cell) 
{
    return cell && (cell->flags & TERRAIN_OVERRIDES_RIVER_MASK) != 0;
}

// Khớp chính xác overridesCoast() = isVolcano()
static inline bool terrain_overrides_coast(const TerrainCell* cell) 
{
    return terrain_is_volcano(cell);
}

// ITerrain.Delegate implementation
typedef struct 
{
    const TerrainCell* delegate_ptr;
} TerrainDelegate;

static inline float delegate_erosion_modifier(const TerrainDelegate* del) 
{
    return del && del->delegate_ptr ? terrain_erosion_modifier(del->delegate_ptr) : 1.0f;
}

static inline bool delegate_is_flat(const TerrainDelegate* del) { return del && terrain_is_flat(del->delegate_ptr); }
static inline bool delegate_is_river(const TerrainDelegate* del) { return del && terrain_is_river(del->delegate_ptr); }
static inline bool delegate_is_shallow_ocean(const TerrainDelegate* del) { return del && terrain_is_shallow_ocean(del->delegate_ptr); }
static inline bool delegate_is_deep_ocean(const TerrainDelegate* del) { return del && terrain_is_deep_ocean(del->delegate_ptr); }
static inline bool delegate_is_coast(const TerrainDelegate* del) { return del && terrain_is_coast(del->delegate_ptr); }
static inline bool delegate_is_lake(const TerrainDelegate* del) { return del && terrain_is_lake(del->delegate_ptr); }
static inline bool delegate_is_wetland(const TerrainDelegate* del) { return del && terrain_is_wetland(del->delegate_ptr); }
static inline bool delegate_is_mountain(const TerrainDelegate* del) { return del && terrain_is_mountain(del->delegate_ptr); }
static inline bool delegate_is_volcano(const TerrainDelegate* del) { return del && terrain_is_volcano(del->delegate_ptr); }
static inline bool delegate_is_overground(const TerrainDelegate* del) { return del && terrain_is_overground(del->delegate_ptr); }
static inline bool delegate_is_submerged(const TerrainDelegate* del) { return del && terrain_is_submerged(del->delegate_ptr); }
static inline bool delegate_overrides_river(const TerrainDelegate* del) { return del && terrain_overrides_river(del->delegate_ptr); }
static inline bool delegate_overrides_coast(const TerrainDelegate* del) { return del && terrain_overrides_coast(del->delegate_ptr); }

#endif