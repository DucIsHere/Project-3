#ifndef FAST_RAND_H
#define FAST_RAND_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
     uint64_t gamma;
     uint64_t seed;
} FastRandom;

static inline uint64_t fr_mix_gamma(uint64_t z)
{
     z = (z ^ (z >> 33)) * 0xFF51AFD7ED558CCDULL;
     z = (z ^ (z >> 33)) * 0xC4CEB9FE1A85EC53ULL;
     return (z ^ (z >> 33)) | 1ULL;
}

static inline int32_t fr_mix32(uint64_t z) {
    z = (z ^ (z >> 33)) * 7109453100751455733ULL;
    return (int32_t)((z ^ (z >> 28)) * -3808689974395783757ULL >> 32);
}

static inline uint64_t fr_next_seed(FastRandom* rng) {
    rng->seed += rng->gamma;
    return rng->seed;
}

// Khởi tạo Random với seed mặc định
static inline void fr_init(FastRandom* rng, uint64_t seed) {
    rng->seed = seed;
    rng->gamma = -7046029254386353131ULL; // Hằng số gốc từ Java
}

// Khởi tạo Random đầy đủ cả seed và gamma custom
static inline void fr_init_full(FastRandom* rng, uint64_t seed, uint64_t gamma) {
    rng->seed = seed;
    rng->gamma = fr_mix_gamma(gamma);
}

// Sinh số nguyên ngẫu nhiên (Full dải 32-bit int)
static inline int32_t fr_next_int(FastRandom* rng) {
    return fr_mix32(fr_next_seed(rng));
}

// Sinh số nguyên kẹp khoảng [0, bound) - Đoạn này tối ưu bit như Java
static inline int32_t fr_next_int_bound(FastRandom* rng, int32_t bound) {
    int32_t r = fr_mix32(fr_next_seed(rng));
    int32_t m = bound - 1;
    
    // Nếu bound là lũy thừa của 2 (2, 4, 8, 16, 64...), dùng toán bit ăn ngay, cực nhanh
    if ((bound & m) == 0) {
        return r & m;
    }
    
    // Nếu không, chạy vòng lặp khử thiên vị phân phối
    int32_t u = (int32_t)((uint32_t)r >> 1);
    while (u + m - (u % bound) < 0) {
        r = fr_mix32(fr_next_seed(rng));
        u = (int32_t)((uint32_t)r >> 1);
    }
    return u % bound;
}

// Sinh float ngẫu nhiên trong khoảng [0.0f, 1.0f) dùng phép nhân thay vì chia
static inline float fr_next_float(FastRandom* rng) {
    uint32_t mixed = (uint32_t)fr_mix32(fr_next_seed(rng));
    return (float)(mixed >> 8) * 5.9604645E-8f;
}

// Sinh Boolean ngẫu nhiên
static inline bool fr_next_boolean(FastRandom* rng) {
    return fr_mix32(fr_next_seed(rng)) < 0;
}

#endif
