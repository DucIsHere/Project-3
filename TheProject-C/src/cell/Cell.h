#ifndef CELL_SOA_H
#define CELL_SOA_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Định nghĩa Căn chỉnh bộ nhớ (32 bytes cho AVX2, 64 bytes cho AVX-512)
#if defined(__AVX512F__)
    #define SIMD_ALIGN 64
#elif defined(__AVX2__)
    #define SIMD_ALIGN 32
#else
    #define SIMD_ALIGN 16
#endif

// Structure of Arrays (SoA) chứa toàn bộ lưới Cell
typedef struct {
    int capacity; // Số lượng cell tối đa (ví dụ: size_x * size_z)

    // Các mảng số thực 32-bit (float)
    float* height;
    float* height_erosion;
    float* sediment;
    float* gradient;
    float* region_moisture;
    float* region_temperature;
    float* continent_id;
    float* continent_edge;
    float* terrain_region_id;
    float* terrain_region_edge;
    float* biome_region_id;
    float* biome_region_edge;
    float* macro_biome_id;
    float* river_mask;
    float* erosion;
    float* weirdness;
    float* sharpness;
    float* temperature;
    float* moisture;
    float* damage;
    float* beach_noise; // @Deprecated

    // Các mảng số nguyên (int32_t)
    int32_t* continent_x;
    int32_t* continent_z;

    // Các mảng Enum/ID đại diện (uint8_t để tiết kiệm RAM tối đa)
    uint8_t* terrain; // Thay cho Terrain object[cite: 7]
    uint8_t* biome;   // Thay cho BiomeType object[cite: 7]
    
    // Mảng Boolean (uint8_t)
    uint8_t* erosion_mask;
} CellSoA;

// Hàm tạo bộ nhớ SoA có Alignment cho SIMD
CellSoA* cell_soa_create(int capacity);

// Hàm giải phóng bộ nhớ
void cell_soa_free(CellSoA* soa);

// Hàm reset các giá trị về mặc định (Tương đương reset() bên Java)[cite: 7]
void cell_soa_reset(CellSoA* soa);

// Hàm reset 1 cell cụ thể theo index
void cell_soa_reset_at(CellSoA* soa, int index);

#endif // CELL_SOA_H
