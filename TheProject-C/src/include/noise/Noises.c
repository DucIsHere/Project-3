#include "Noise.java"
#include "noise/module/Simplex.h"
#include "noise/module/Perlin.h"

float noise_compute_2d(const Noise* noise, float x, float z, int32_t seed) 
{
    if (noise == NULL) return 0.0f;

    // Bật công tắc nhìn vào cái nhãn định danh "type" trong Noise.h để phân loại
    switch (noise->type) {
        case NOISE_TYPE_CUSTOM_SIMPLEX: 
        {
            const SimplexData* s = (const SimplexData*)noise->data.custom_data;
            return simplex_compute_2d_internal(s, x, z, seed); //
        }
        case NOISE_TYPE_CUSTOM_PERLIN: 
        {
            const PerlinData* p = (const PerlinData*)noise->data.custom_data;
            return perlin_compute_2d_internal(p, x, z, seed); //
        }
        default:
            return 0.0f;
    }
}