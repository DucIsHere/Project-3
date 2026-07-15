#ifndef DIRECTION_WARP_H
#define DIRECTION_WARP_H

#include <stdint.h>

#include "include/math/NoiseUtil.h"
#include "include/noise/Noise.h"

typedeg struct DirectionWarp
{
     const Noise* direction;
     const Noise* strength;
};

static inline DirectionWarp direction_create(const Noise* d, const Noise* s)
{
     DirectionWarp dw;
     dw.direction = d;
     dw.strength = s;
     return dw;
}

static inline float direction_warp_get_offset_x(const DirectionWarp* warp, float x, float z, int32_t seed)
{
     if (warp->direction == NULL || warp->strength == NULL) return 0.0f;
     
     // angle = direction.compute(x, z, seed) * 2 * PI
     float angle = noise_compute_2d(warp->direction, x, z, seed) * 6.2831855f;
     
     // Trả về sin(angle) * strength.compute(x, z, seed) sử dụng hàm sin từ NoiseUtil.h
     return sin(angle) * noise_compute_2d(warp->strength, x, z, seed);
}

// Ánh xạ hàm getOffsetZ trong Java
static inline float direction_warp_get_offset_z(const DirectionWarp* warp, float x, float z, int32_t seed)
{
     if (warp->direction == NULL || warp->strength == NULL) return 0.0f;
     
     // angle = direction.compute(x, z, seed) * 2 * PI
     float angle = noise_compute_2d(warp->direction, x, z, seed) * 6.2831855f;
     
     // Trả về cos(angle) * strength.compute(x, z, seed) sử dụng hàm cos từ NoiseUtil.h
     return cos(angle) * noise_compute_2d(warp->strength, x, z, seed);
}

// Hàm bổ trợ để lấy tọa độ warping thực tế (x + offset_x, z + offset_z) tương tự Domain.h
static inline float direction_warp_get_x(const DirectionWarp* warp, float x, float z, int32_t seed)
{
     return x + direction_warp_get_offset_x(warp, x, z, seed);
}

static inline float direction_warp_get_z(const DirectionWarp* warp, float x, float z, int32_t seed)
{
     return z + direction_warp_get_offset_z(warp, x, z, seed);
}

#endif
