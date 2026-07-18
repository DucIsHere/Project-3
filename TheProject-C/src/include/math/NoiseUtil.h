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

static inline float abs(float x)
{
    return x < 0 ? -x : x;
}

static inline float min(float a, float b)
{
    return (a < b) ? a : b;
}

static inline float max(float a, float b)
{
    return (a > b) ? a : b;
}

static inline float clamp(float value, float min, float max)
{
     return value < min ? min : (value > max ? max : value);
}

static inline int32_t floorf(float f)
{
    int32_t i = (int32_t)f;
    return f < (float)i ? i - 1 : i;
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

static inline float interpHermite(float f)
{
    return f * f * (3.0F - 2.0F * f); // Khớp đét Hermite Spline bậc 3
}

static inline float interpQuintic(float f)
{
    // Khớp đét Perlin Quintic bậc 5: f^3 * (f * (f * 6 - 15) + 10)
    return f * f * f * (f * (f * 6.0F - 15.0F) + 10.0F); 
}

static inline int32_t hash(int32_t x, int32_t y)
{
    int32_t hash = x;
    hash ^= 31337 * y;
    hash = hash * hash * hash * 60493;
    hash ^= hash >> 13;
    return hash;
}

static inline int32_t hash_2d(int32_t seed, int32_t x, int32_t y) {
    int32_t hash = seed;
    hash ^= 1619 * x;
    hash ^= 31337 * y;
    hash = hash * hash * hash * 60493;
    hash ^= hash >> 13;
    return hash;
}

static inline Vec2f cell(int32_t seed, int32_t x, int32_t y) {
    return CELL_2D[hash_2d(seed, x, y) & 0xFF];
}

static inline float valCoord2D(int32_t seed, int32_t x, int32_t y)
{
    int32_t n = seed;
    n ^= 1619 * x;
    n ^= 31337 * y;
    return n * n * n * 60493 / 2.14748365E9F;
}

static inline float sin(float rad) {
    int32_t index = (int32_t)(rad * (SIN_COUNT / PI2)); 
    return SIN[index & SIN_MASK];
}

static inline float cos(float rad) 
{
    return sin(rad + 1.5708f);
}

static inline float coord2D(int32_t seed, int32_t x, int32_t y)
{
    int32_t hash = seed;
    hash ^= 1619 * x;
    hash ^= 31337 * y;
    hash = hash * hash * hash * 60493;
    hash ^= hash >> 13;
    return GRAD_2D[hash & 0x7];
}

static inline Vec2f coord2D_24(int32_t seed, int32_t x, int32_t y)
{
    int32_t hash = seed;
    hash ^= 1619 * x;
    hash ^= 31337 * y;
    hash = hash * hash * hash * 60493;
    hash ^= hash >> 13;
    int32_t selector24 = (int32_t)((hash & 0x3FFFFF) * 1.3333334F) & 0x1F;
    return GRAD_2D_24[selector24];

}

__attribute__((visibility("default"))) void bind_native_noise_constants(float* java_cell_2d, float* java_grad_2d, float* java_grad_2d_24);

#endif
