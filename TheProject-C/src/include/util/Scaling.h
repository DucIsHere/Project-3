#ifndef SCALING_H
#define SCALING_H

#include <stdint.h>
#include <stdbool.h>

#include "include/math/NoiseUtil.h" // Ăn trực tiếp vào file NoiseUtil.h thực tế của ông

// Định nghĩa hàm chia an toàn nội bộ bằng float cho Scaling
static inline float scaling_safe_div(float a, float b) {
    if (b == 0.0f) return 0.0f;
    return a / b;
}

// Struct Scaling: Giữ nguyên int64_t cho các mốc Block Y để khớp Config.h
// Nhưng toàn bộ hệ số tỉ lệ (unit, ground, water, range) phải là float để khớp NoiseUtil.h
typedef struct {
    int64_t world_height;    // 64-bit khớp với cf.levels.world_height
    float unit;              // float khớp với toán địa hình
    int64_t water_y;         // 64-bit mốc block Y
    int64_t ground_y;        // 64-bit mốc block Y
    int64_t ground_level;    // 64-bit
    int64_t water_level;     // 64-bit
    float ground;            // float (tỉ lệ 0.0 -> 1.0)
    float water;             // float (tỉ lệ 0.0 -> 1.0)
    float elevation_range;   // float
} Scaling;

// Hàm khởi tạo nhận vào int64_t từ Conf.c nhưng tính toán ra tỉ lệ float sạch sẽ
static inline Scaling scaling_make(int64_t height, int64_t sea_level) {
    Scaling s;
    s.world_height = (height > 1) ? height : 1;
    s.unit = scaling_safe_div(1.0f, (float)s.world_height);
    s.water_level = sea_level;
    s.ground_level = sea_level + 1;
    
    s.water_y = (s.water_level - 1 < s.world_height) ? (s.water_level - 1) : s.world_height;
    s.ground_y = (s.ground_level - 1 < s.world_height) ? (s.ground_level - 1) : s.world_height;
    
    // Gọi hàm chia an toàn bằng float, băm khớp với logic NoiseUtil.div gốc
    s.ground = scaling_safe_div((float)s.ground_y, (float)s.world_height);
    s.water = scaling_safe_div((float)s.water_y, (float)s.world_height);
    s.elevation_range = 1.0f - s.water;
    
    return s;
}

// scale(float value) -> Trả về int32_t để ném vào PosUtil pack bit nếu cần!
static inline int32_t scaling_scale_to_int32(const Scaling* s, float value) {
    return (int32_t)(value * (float)s->world_height);
}

// scale(float value) -> Trả về int64_t nếu muốn lấy tọa độ block Y hệ 64-bit
static inline int64_t scaling_scale_to_int64(const Scaling* s, float value) {
    return (int64_t)(value * (float)s->world_height);
}

// elevation(float value) -> Ăn khớp hoàn hảo với kiểu float của NoiseUtil.h
static inline float scaling_elevation_float(const Scaling* s, float value) {
    if (value <= s->water) {
        return 0.0f;
    }
    return (value - s->water) / s->elevation_range;
}

// elevation(int64_t y) -> Nhận vào y 64-bit từ cấu hình nhưng tính toán ra tỉ lệ float
static inline float scaling_elevation_int(const Scaling* s, int64_t y) {
    if (y <= s->water_y) {
        return 0.0f;
    }
    // Tránh dùng hàm nhân scale trung gian gây rườm rà, tính thẳng bằng float kịch tốc độ
    return (float)(y - s->water_y) * s->unit / s->elevation_range;
}

// blocks(int64_t level)
static inline float scaling_blocks(const Scaling* s, int64_t level) {
    return scaling_safe_div((float)level, (float)s->world_height);
}

// water(int64_t amount)
static inline float scaling_water(const Scaling* s, int64_t amount) {
    return scaling_safe_div((float)(s->water_y + amount), (float)s->world_height);
}

// ground(int64_t amount)
static inline float scaling_ground(const Scaling* s, int64_t amount) {
    return scaling_safe_div((float)(s->ground_y + amount), (float)s->world_height);
}

#endif // SCALING_H