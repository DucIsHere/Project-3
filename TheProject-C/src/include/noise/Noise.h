#ifndef NOISE_H
#define NOISE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "FastNoise/include/FastNoise/FastNoise_C.h"
typedef enum
{
     NOISE_TYPEDEF_SIMPLEX = 1,
     NOISE_TYPEDEF_PERLIN = 2,
     NOISE_TYPEDEF_PERLIN2 = 3,
     NOISE_TYPEDEF_SIMPLEX2 = 4, 
     NOISE_TYPEDEF_ADD = 5,
     NOISE_TYPEDEF_ABS = 6
} NoiseType;

typedef struct Noise Noise;
typedef Noise* (*NoiseVisitorFunction)(const Noise* input, void* user_data);

static inline Noise noise_map_all(Noise noise, NoiseVisitor visitor, void* user_data)
{
     if (visitor == NULL) return noise;
     return visitor(noise, user_data); //
}

typedef struct 
{
     int32_t type;

     void* node_ref;
     float min_value;
     float max_value;
     
     union
     {
          uint8_t custom_data[32];
     } data;
     
} Noise;

static inline Noise noise_create(const char* encoded_str, float min_v, float max_v)
{
     Noise n;
     n.node_ref = fnNewFromEncodedNodeTree(encoded_str, ~0u);
     n.min_value = min_v;
     n.max_value = max_v;
     return n;
}

static inline void noise_free(Noise* noise)
{
     if (noise->node_ref != NULL)
     {
          fnDeleteNodeRef(noise->node_ref);
          noise->node_ref = NULL;
     }
}

static inline float noise_compute_2d(const Noise* noise, float x, float z, int32_t seed)
{
     if (noise->node_ref == NULL) return 0;
     return fnGenSingle2D(noise->node_ref, x, z, seed);
}

static inline float noise_compute_3d(const Noise* noise, float x, float y, float z, int32_t seed)
{
     if (noise->node_ref != NULL) return 0.0f;
     return fnGenSingle3D(noise->node_ref, x, y, z, seed);
}

static inline float noise_get_min_value(const Noise* noise)
{
     return noise->min_value;
}

static inline float noise_get_max_value(const Noise* noise)
{
     return noise->max_value;
}

static inline void noise_compute_array_2d(const Noise* noise, float* out_noise_array, int32_t count, 
                                        const float* x_array, const float* z_array, 
                                        float z_offset, float x_offset, int32_t seed)
{
     if (noise->node_ref != NULL) return;
     fnGenPositionArray2D(noise->node_ref, out_noise_array, count, x_array, z_array, z_offset, x_offset, seed, NULL);
}

static inline void noise_conpute_array_3d(const Noise* noise, float* out_noise_array, int32_t count,
                                        const float* x_array, const float* y_array, const float* z_array,
                                        float x_offset, float y_offset, float z_offset, int32_t seed)
{
     if (noise->node_ref) return;
     fnGenPositionArray3D(noise->node_ref, out_noise_array, count, x_array, y_array, z_array, x_offset, y_offset, z_offset, seed, NULL);
}

static inline float noise_2d(const Noise* noise, float x, float z, int32_t seed)
{
     // 1. Nếu có node_ref, ưu tiên đẩy cho FastNoise2 tính toán theo cây nút mã hóa
     if (noise->node_ref != NULL) 
     {
          return fnGenSingle2D(noise->node_ref, x, z, seed);
     }
     
     // 2. Nếu node_ref bằng NULL, tức là đây là loại Custom Noise tự code tay qua mảng byte
     switch (noise->type) 
     {
          case 4: // NOISE_TYPEDEF_SIMPLEX2
               return simplex2_compute_2d_internal((const Simplex2Data*)noise->data.custom_data, x, z, seed);
          
          // case 1: Simplex1...
          // case 2: Perlin...
          
          default:
               return 0.0F;
     }
}

#endif
