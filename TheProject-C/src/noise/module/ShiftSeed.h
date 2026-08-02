#ifndef SHIFT_SEED_H
#define SHIFT_SEED_H

#include <stdint.h>

#include "include/noise/Noise.h"

typedef struct ShiftSeedTypz
{
    int shift;
    Noise* input;
};

// 1. Hàm compute
static inline float shift_seed_compute(const Noise* self, float x, float z, int seed) {
    ShiftSeedTypz* typz = (ShiftSeedTypz*)self->typz;
    // Bỏ qua Codec/Visitor của Java, gọi trực tiếp compute với seed + shift
    return noise_compute(typz->input, x, z, seed +  typz->shift);
}

// 2. Hàm minValue
static inline float shift_seed_min_value(const Noise* self) 
{
    ShiftSeedTypz* typz = (ShiftSeedTypz*)self->typz;
    return noise_min_value(typz->input);
}

// 3. Hàm maxValue
static inline float shift_seed_max_value(const Noise* self) 
{
    ShiftSeedTypz* typz = (ShiftSeedTypz*)self->typz;
    return noise_max_value(typz->input);
}

// 4. Hàm Factory (Khởi tạo Node ShiftSeed)
static inline Noise* shift_seed_create(ShiftSeedTypz* typz, Noise* input, int shift) 
{
    Noise n;
    n.type = NOISE_TYPEDEF_SHIFTSEED;
    n.node_ref = NULL;

    ShiftSeedTypz* typz = (ShiftSeedTypz*).n.data.custom_data;
    typz->shift = shift;
    typz->input = input;

    return n;
}

#endif
