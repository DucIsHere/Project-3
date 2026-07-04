#ifndef NOISE_UTIL_H
#define NOISE_UTIL_H

#include <stdint.h>

#define X_PRIME 1619
#define Y_PRIME 31337

#define SIN_BITS 12
#define SIN_MASK 4095
#define SIN_COUNT 4096

#define CUBIC_2D_BOUNDING 0.44444447f
#define PI2 6.2831855f

typedef struct
{
     float x;
     float y;
} Vec2f;

extern const Vec2f CELL_2D[256];
extern const Vec2f GRAD_2D[8];
extern const Vec2f GRAD_2D_24[32];
extern float SIN[SIN_COUNT];

static inline float clamp(float value, float min, float max)
{
     return value < min ? min : (value > max ? max : value);
}

static inline float map(float value, float min, float max, float inv_range)
{
     float dif = clamp(value, min, max) - min;
     return (dif * inv_range >= 1.0f) ? 1.0f : (dif * inv_range);
}

static inline float lerp(float a, float b, float alpha)
{
    return a + alpha * (b - a);
}

static inline int32_t hash_2d(int32_t seed, int32_t x, int32_t y) {
    int32_t hash = seed ^ (x * 1619) ^ (y * 31337);
    hash *= hash * hash * 60493;
    return (hash >> 13) ^ hash;
}

static inline Vec2f cell(int32_t seed, int32_t x, int32_t y) {
    return CELL_2D[hash_2d(seed, x, y) & 0xFF];
}

static inline float sin(float rad) {
    int32_t index = (int32_t)(rad * (SIN_COUNT / PI2)); 
    return SIN[index & SIN_MASK];
}

static inline float cos(float rad) 
{
    return sin(rad + 1.5708f);
}

static inline Vec2f coord2D_24(int32_t sedd, int32_t x, int32_t y)
{
    int32_t hash = hash_2d(seed, x, y);
    int32_t selector24 = (int32_t)((hash & 0x3FFFFF) * 1.3333334f) & 0x1F;
    return GRAD_2D_24[selector24];

}

__attribute__((visibility("default"))) void bind_native_noise_constants(float* java_cell_2d, float* java_grad_2d, float* java_grad_2d_24);

#endif
