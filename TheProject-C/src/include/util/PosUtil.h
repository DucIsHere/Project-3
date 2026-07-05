#ifndef POS_UTIL_H
#define POS_UTIL_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static inline uint32_t pu_float_to_raw_int(float f) 
{
    uint32_t i;
    memcpy(&i, &f, sizeof(float));
    return i;
}

static inline float pu_raw_int_to_float(uint32_t i) 
{
    float f;
    memcpy(&f, &i, sizeof(uint32_t));
    return f;
}

// --- CÁC HÀM PACK / UNPACK BIT (Dùng static inline để kịch tốc độ) ---

// packMix(int left, float right)
static inline uint64_t pu_pack_mix_int_float(int32_t left, float right) 
{
    return ((uint64_t)pu_float_to_raw_int(right) & 0xFFFFFFFFULL) | 
           (((uint64_t)left & 0xFFFFFFFFULL) << 32);
}

// packMix(float left, int right)
static inline uint64_t pu_pack_mix_float_int(float left, int32_t right) 
{
    return ((uint64_t)right & 0xFFFFFFFFULL) | 
           (((uint64_t)pu_float_to_raw_int(left) & 0xFFFFFFFFULL) << 32);
}

// pack(int left, int right)
static inline uint64_t pu_pack_int(int32_t left, int32_t right) 
{
    return ((uint64_t)right & 0xFFFFFFFFULL) | 
           (((uint64_t)left & 0xFFFFFFFFULL) << 32);
}

// pack(float left, float right) -> Trong Java gốc của ông đang là ép kiểu thẳng float sang long, C làm y hệt
static inline uint64_t pu_pack_float(float left, float right) 
{
    return ((uint64_t)right & 0xFFFFFFFFULL) | 
           (((uint64_t)left & 0xFFFFFFFFULL) << 32);
}

// unpackLeft(long packed) -> Trả về int
static inline int32_t pu_unpack_left(uint64_t packed) 
{
    return (int32_t)(packed >> 32);
}

// unpackRight(long packed) -> Trả về int
static inline int32_t pu_unpack_right(uint64_t packed)
{
    return (int32_t)(packed & 0xFFFFFFFFULL);
}

// packf(float left, float right)
static inline uint64_t pu_packf(float left, float right) 
{
    return ((uint64_t)pu_float_to_raw_int(right) & 0xFFFFFFFFULL) | 
           (((uint64_t)pu_float_to_raw_int(left) & 0xFFFFFFFFULL) << 32);
}

// unpackLeftf(long packed) -> Trả về float
static inline float pu_unpack_leftf(uint64_t packed) 
{
    return pu_raw_int_to_float((uint32_t)(packed >> 32));
}

// unpackRightf(long packed) -> Trả về float
static inline float pu_unpack_rightf(uint64_t packed) 
{
    return pu_raw_int_to_float((uint32_t)(packed & 0xFFFFFFFFULL));
}

// --- CÁC HÀM CONTAINS ---

static inline bool pu_contains_int(int32_t x, int32_t z, int32_t x1, int32_t z1, int32_t x2, int32_t z2) 
{
    return x >= x1 && x < x2 && z >= z1 && z < z2;
}

static inline bool pu_contains_float(float x, float z, float x1, float z1, float x2, float z2) 
{
    return x >= x1 && x < x2 && z >= z1 && z < z2;
}

#endif
