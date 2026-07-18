#ifndef PERLIN_H
#define PERLIN_H

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "function/Interpolation.h"
#include "include/noise/Noise.h"

/ Mảng SIGNALS lấy chuẩn đét từ Perlin.java
static const float PERLIN_SIGNALS[] = { 
    1.0F, 0.9F, 0.83F, 0.75F, 0.64F, 0.62F, 0.61F 
};

// Cấu trúc dữ liệu Perlin nằm gọn gàng trong 32 bytes custom_data thô
typedef struct {
    float frequency;
    int32_t octaves;
    float lacunarity;
    float gain;
    InterpolationType interpolation; // Khớp chỉ mục số nguyên (0, 1, 2) từ Java sang
} PerlinData;

// --- HÀM TÍNH GRADIENT CHO PERLIN 2D (Đồng bộ chuẩn theo logic NoiseUtil.gradCoord2D bên Java) ---
static inline float perlin_util_grad_coord_2d(int32_t seed, int32_t x, int32_t y, float x_f, float y_f) {
    // 1. Băm bit tọa độ x, y để lấy hash ngẫu nhiên từ NoiseUtil.h
    int32_t hash = hash_2d(seed, x, y); //
    
    // 2. Ép index ăn vào mảng GRAD_2D (Perlin cơ bản có 8 hướng, dùng phép bit hash & 7)
    int32_t index = hash & 7;
    
    // 3. Dot Product (tích vô hướng) giữa vector ngẫu nhiên nhặt trong mảng tĩnh GRAD_2D và khoảng cách float
    return (GRAD_2D[index].x * x_f) + (GRAD_2D[index].y * y_f); //
}

// Hàm tính biên độ đỉnh núi tối đa cho Perlin
static inline float perlin_calc_max_val(int32_t octaves, float gain) {
    float gain_acc = 1.0F;
    float sum = 0.0F;
    for (int32_t i = 0; i < octaves; ++i) {
        sum += (i < 7 ? PERLIN_SIGNALS[i] : 1.0F) * gain_acc; //
        gain_acc *= gain; //
    }
    return sum;
}

// Hàm sample tính toán Perlin 2D đơn tầng (Dịch 100% từ hàm static sample bên Perlin.java)
static inline float perlin_raw_single_2d(float x, float y, int32_t seed, InterpolationType interp) {
    // Dùng floorf của C để lấy phần nguyên (ép về tọa độ ô lưới)
    int32_t x2 = (int32_t)floorf(x); //
    int32_t y2 = (int32_t)floorf(y); //
    int32_t x3 = x2 + 1; //
    int32_t y3 = y2 + 1; //

    float xd0 = x - (float)x2; //
    float yd0 = y - (float)y2; //
    float xd2 = xd0 - 1.0F;    //
    float yd2 = yd0 - 1.0F;    //

    // Áp dụng mảng con trỏ hàm nội suy kịch tốc siêu phẳng từ Interpolation.h
    float xs = interpolation_apply(interp, xd0); //
    float ys = interpolation_apply(interp, yd0); //

    // Tính toán dốc hướng tại 4 đỉnh ô vuông
    float grad_00 = perlin_util_grad_coord_2d(seed, x2, y2, xd0, yd0); //
    float grad_10 = perlin_util_grad_coord_2d(seed, x3, y2, xd2, yd0); //
    float grad_01 = perlin_util_grad_coord_2d(seed, x2, y3, xd0, yd2); //
    float grad_11 = perlin_util_grad_coord_2d(seed, x3, y3, xd2, yd2); //

    // Nội suy tuyến tính phẳng (Hàm lerp hốt trực tiếp từ NoiseUtil.h)
    float xf0 = lerp(grad_00, grad_10, xs); //
    float xf2 = lerp(grad_01, grad_11, xs); //

    return lerp(xf0, xf2, ys); //
}

// Vòng lặp uốn chập tầng Octaves của Perlin theo cấu trúc phân tầng địa hình
static inline float perlin_compute_2d_internal(const PerlinData* p, float x, float z, int32_t seed) {
    float value = 0.0F;
    float freq = p->frequency; //
    float amplitudes = 1.0F;

    for (int32_t i = 0; i < p->octaves; ++i) { //
        float signal = i < 7 ? PERLIN_SIGNALS[i] : 1.0F; //
        // Chạy qua hàm sample đơn tầng, seed tăng dần theo tầng octave giống hệt Java
        value += perlin_raw_single_2d(x * freq, z * freq, seed + i, p->interpolation) * signal * amplitudes; //
        freq *= p->lacunarity; //
        amplitudes *= p->gain; //
    }
    return value;
}

// Hàm Factory tạo Perlin trả về Struct Noise phẳng cho hệ thống lớn dùng
static inline Noise noises_perlin_create(int32_t scale, int32_t octaves, float lacunarity, float gain, InterpolationType interp) {
    Noise n;
    n.type = NOISE_TYPEDEF_PERLIN;
    n.node_ref = NULL;
    
    PerlinData* p = (PerlinData*)n.data.custom_data;
    p->frequency = 1.0F / (float)scale; // Chuyển đổi từ scale sang frequency hệt bên Java
    p->octaves = octaves;
    p->lacunarity = lacunarity;
    p->gain = gain;
    p->interpolation = interp; //
    
    float max_val = perlin_calc_max_val(octaves, gain); //
    n.min_value = -max_val;
    n.max_value = max_val;
    return n;
}

#endif
