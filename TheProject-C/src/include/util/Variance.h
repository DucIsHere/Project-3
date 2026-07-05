#ifndef VARIANCE_H
#define VARIANCE_H

#include <stdint.h>
#include "FastRandom.h" // Include con hàng khét tiếng của ông vào đây!

// Struct Variance: Nặng đúng 8 bytes (2 số float)
typedef struct 
{
    float min;
    float range;
} Variance;

// --- CÁC HÀM KHỞI TẠO (FACTORY METHODS) ---

static inline Variance variance_of(float min, float range) 
{
    Variance v;
    v.min = min;
    v.range = range;
    return v;
}

static inline Variance variance_min(float min) {
    return variance_of(min, 1.0f - min);
}

static inline Variance variance_range(float range) 
{
    return variance_of(1.0f - range, range);
}

// --- CÁC HÀM ÁP DỤNG BIẾN ĐỘNG (LOGIC METHODS) ---

static inline float variance_apply(const Variance* v, float value) 
{
    return v->min + (value * v->range);
}

static inline float variance_apply_scaled(const Variance* v, float value, float scaler) 
{
    return variance_apply(v, value) * scaler;
}

// public float next(FastRandom random) -> Gọi chuẩn hàm fr_next_float của ông!
static inline float variance_next(const Variance* v, FastRandom* rng) 
{
    return variance_apply(v, fr_next_float(rng));
}

// public float next(FastRandom random, float scalar)
static inline float variance_next_scaled(const Variance* v, FastRandom* rng, float scalar) 
{
    return variance_apply_scaled(v, fr_next_float(rng), scalar);
}

#endif // VARIANCE_H