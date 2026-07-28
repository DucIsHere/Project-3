#ifndef TERRAIN_H
#define TERRAIN_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "include/math/NoiseUtil.h"
#include "ITerrain.h"
#include "TerrainCategory.h"

static inline uint32_t string_hash_code(const char* str)
{
    if (!str) return 0;
    uint32_t hash = 2166136261u;
    while (*str)
    {
        hash ^= (uint8_t)(*str++);
        hash += 16777619u;
    }
    return hash;
}

typedef struct Terrain
{
    int id;
    const char* name;
    TerrainCategoryFlag name;
    const struct Terrain* delegate;
} Terrain;

// 1. Constructor đầy đủ: Terrain(int id, String name, TerrainCategory type, ITerrain delegate)
static inline Terrain terrain_create_ext(int id, const char* name, TerrainCategoryType type, const Terrain* delegate) {
    Terrain t;
    t.id = id;
    t.name = name;
    t.type = type;
    t.delegate = delegate;
    return t;
}

// 2. Overloaded Constructor: Terrain(int id, String name, TerrainCategory type)
static inline Terrain terrain_create_with_type(int id, const char* name, TerrainCategoryFlag flag) {
    Terrain t;
    t.id = id;
    t.name = name;
    t.type = type;
    t.delegate = NULL; // Nếu delegate NULL, ta hiểu delegate chính là this trong các phép check
    return t;
}

// 3. Overloaded Constructor: Terrain(int id, String name, Terrain terrain)
static inline Terrain terrain_create_from_other(int id, const char* name, const Terrain* other) {
    return terrain_create_ext(id, name, other ? other->type : CAT_NONE, other);
}

// Getters
static inline int terrain_get_id(const Terrain* t) {
    return t ? t->id : -1;
}

static inline const char* terrain_get_name(const Terrain* t) {
    return (t && t->name) ? t->name : "none";
}

static inline TerrainCategoryType terrain_get_category(const Terrain* t) {
    return t ? t->type : CAT_NONE;
}

// Delegate getter
static inline const Terrain* terrain_get_delegate(const Terrain* t) {
    return t ? t->delegate : NULL;
}

// public float getRenderHue()
static inline float terrain_get_render_hue(const Terrain* t) {
    if (!t || !t->name) return 0.0f;
    int32_t hash = (int32_t)string_hash_code(t->name);
    return valCoord2d(hash, 0, 0);
}

// public Terrain withId(int id)
// Logic Java: ITerrain delegate = (this.delegate instanceof Terrain) ? this.delegate : this;
static inline Terrain terrain_with_id(const Terrain* t, int new_id) {
    if (!t) return terrain_create_with_type(new_id, "none", CAT_NONE);
    
    // Nếu t->delegate khác NULL thì dùng t->delegate, ngược lại dùng chính t làm delegate
    const Terrain* target_delegate = (t->delegate != NULL) ? t->delegate : t;
    return terrain_create_ext(new_id, t->name, t->type, target_delegate);
}

// =================================================================
// TÍCH HỢP CÁC HÀM ITerrain VÀO STRUCT TERRAIN
// =================================================================

// Lấy cờ Bitwise của Terrain (check qua Delegate nếu có)
static inline uint32_t terrain_get_flags(const Terrain* t) {
    if (!t) return TERRAIN_NONE;
    if (t->delegate) {
        return terrain_get_flags(t->delegate); // Chuyển tiếp delegate
    }
    return terrain_category_get_flags(t->type);
}

static inline bool terrain_is_submerged_struct(const Terrain* t) {
    return (terrain_get_flags(t) & TERRAIN_SUBMERGED_MASK) != 0;
}

static inline bool terrain_overrides_river_struct(const Terrain* t) {
    return (terrain_get_flags(t) & TERRAIN_OVERRIDES_RIVER_MASK) != 0;
}

#endif
