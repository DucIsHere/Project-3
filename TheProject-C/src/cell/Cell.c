#include "Cell.h"

// Hàm cấp phát bộ nhớ căn chỉnh (Aligned Memory Allocation)
static inline void* aligned_alloc_helper(size_t size) {
#if defined(_MSC_VER)
    return _aligned_malloc(size, SIMD_ALIGN);
#else
    void* ptr = NULL;
    if (posix_memalign(&ptr, SIMD_ALIGN, size) != 0) return NULL;
    return ptr;
#endif
}

static inline void aligned_free_helper(void* ptr) {
#if defined(_MSC_VER)
    _aligned_free(ptr);
#else
    free(ptr);
#endif
}

#define ALLOC_ARRAY(ptr, type, cap) \
    ptr = (type*)aligned_alloc_helper((cap) * sizeof(type))

#define FREE_ARRAY(ptr) \
    if (ptr) { aligned_free_helper(ptr); ptr = NULL; }

CellSoA* cell_soa_create(int capacity) {
    CellSoA* soa = (CellSoA*)malloc(sizeof(CellSoA));
    soa->capacity = capacity;

    // Cấp phát từng mảng riêng biệt có căn chỉnh bộ nhớ
    ALLOC_ARRAY(soa->height, float, capacity);
    ALLOC_ARRAY(soa->height_erosion, float, capacity);
    ALLOC_ARRAY(soa->sediment, float, capacity);
    ALLOC_ARRAY(soa->gradient, float, capacity);
    ALLOC_ARRAY(soa->region_moisture, float, capacity);
    ALLOC_ARRAY(soa->region_temperature, float, capacity);
    ALLOC_ARRAY(soa->continent_id, float, capacity);
    ALLOC_ARRAY(soa->continent_edge, float, capacity);
    ALLOC_ARRAY(soa->terrain_region_id, float, capacity);
    ALLOC_ARRAY(soa->terrain_region_edge, float, capacity);
    ALLOC_ARRAY(soa->biome_region_id, float, capacity);
    ALLOC_ARRAY(soa->biome_region_edge, float, capacity);
    ALLOC_ARRAY(soa->macro_biome_id, float, capacity);
    ALLOC_ARRAY(soa->river_mask, float, capacity);
    ALLOC_ARRAY(soa->erosion, float, capacity);
    ALLOC_ARRAY(soa->weirdness, float, capacity);
    ALLOC_ARRAY(soa->sharpness, float, capacity);
    ALLOC_ARRAY(soa->temperature, float, capacity);
    ALLOC_ARRAY(soa->moisture, float, capacity);
    ALLOC_ARRAY(soa->damage, float, capacity);
    ALLOC_ARRAY(soa->beach_noise, float, capacity);

    ALLOC_ARRAY(soa->continent_x, int32_t, capacity);
    ALLOC_ARRAY(soa->continent_z, int32_t, capacity);

    ALLOC_ARRAY(soa->terrain, uint8_t, capacity);
    ALLOC_ARRAY(soa->biome, uint8_t, capacity);
    ALLOC_ARRAY(soa->erosion_mask, uint8_t, capacity);

    // Gán giá trị mặc định ban đầu
    cell_soa_reset(soa);

    return soa;
}

void cell_soa_free(CellSoA* soa) {
    if (!soa) return;

    FREE_ARRAY(soa->height);
    FREE_ARRAY(soa->height_erosion);
    FREE_ARRAY(soa->sediment);
    FREE_ARRAY(soa->gradient);
    FREE_ARRAY(soa->region_moisture);
    FREE_ARRAY(soa->region_temperature);
    FREE_ARRAY(soa->continent_id);
    FREE_ARRAY(soa->continent_edge);
    FREE_ARRAY(soa->terrain_region_id);
    FREE_ARRAY(soa->terrain_region_edge);
    FREE_ARRAY(soa->biome_region_id);
    FREE_ARRAY(soa->biome_region_edge);
    FREE_ARRAY(soa->macro_biome_id);
    FREE_ARRAY(soa->river_mask);
    FREE_ARRAY(soa->erosion);
    FREE_ARRAY(soa->weirdness);
    FREE_ARRAY(soa->sharpness);
    FREE_ARRAY(soa->temperature);
    FREE_ARRAY(soa->moisture);
    FREE_ARRAY(soa->damage);
    FREE_ARRAY(soa->beach_noise);

    FREE_ARRAY(soa->continent_x);
    FREE_ARRAY(soa->continent_z);

    FREE_ARRAY(soa->terrain);
    FREE_ARRAY(soa->biome);
    FREE_ARRAY(soa->erosion_mask);

    free(soa);
}

// Reset toàn bộ mảng chuẩn đét 1:1 theo Constructor Java[cite: 7]
void cell_soa_reset(CellSoA* soa) {
    int cap = soa->capacity;
    for (int i = 0; i < cap; i++) {
        cell_soa_reset_at(soa, i);
    }
}

void cell_soa_reset_at(CellSoA* soa, int i) {
    soa->height[i] = 0.0f;
    soa->height_erosion[i] = 0.0f;
    soa->sediment[i] = 0.0f;
    soa->gradient[i] = 0.0f;
    soa->region_moisture[i] = 0.5f;     // Default bên Java[cite: 7]
    soa->region_temperature[i] = 0.5f;  // Default bên Java[cite: 7]
    soa->continent_id[i] = 0.0f;
    soa->continent_edge[i] = 0.0f;
    soa->terrain_region_id[i] = 0.0f;
    soa->terrain_region_edge[i] = 0.0f;
    soa->biome_region_id[i] = 0.0f;
    soa->biome_region_edge[i] = 1.0f;   // Default bên Java[cite: 7]
    soa->macro_biome_id[i] = 0.0f;
    soa->river_mask[i] = 1.0f;          // Default bên Java[cite: 7]
    soa->erosion[i] = 0.0f;
    soa->weirdness[i] = 0.0f;
    soa->sharpness[i] = 0.0f;
    soa->temperature[i] = 0.0f;
    soa->moisture[i] = 0.0f;
    soa->damage[i] = 0.0f;
    soa->beach_noise[i] = 0.0f;

    soa->continent_x[i] = 0;
    soa->continent_z[i] = 0;

    soa->erosion_mask[i] = 0;            // false[cite: 7]
    soa->terrain[i] = 0;                 // TerrainType.NONE[cite: 7]
    soa->biome[i] = 0;                   // BiomeType.GRASSLAND (ví dụ enum 0)[cite: 7]
}