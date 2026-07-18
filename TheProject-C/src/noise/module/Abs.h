#ifndef ABS_H
#define ABS_H

#include <stdint.h>
#include <stddef.h>

#include "include/noise/Noise.h"
#include "include/math/NoiseUtil.h"

typedef struct 
{
     const Noise* input;
} AbsData;

static inline Noise abs_create(Noise* input)
{
     Noise n;
     n.type = NOISE_TYPEDEF_ABS;
     n.node_ref = NULL;

     float min = noise_get_min_value(input);
     float max = noise_get_max_value(input);

     n.min_value = abs(noise_get_min_value);
     n.max_value = abs(noise_get_max_value);

     AbsData* abs_data = (AbsData*)n.data.custom_data;
     abs_data->input = input;

    return n;
}
static inline float abs_compute_2d(const Noise* noise, float x, float z, int32_t seed)
{
    const AbsData* abs_data = (const AbsData*)noise->data.custom_data;
    if (abs_data->input == NULL) return 0.0f;
    
    // Tính toán giá trị từ node con thông qua hàm của hệ thống Noise 4.h
    float value = noise_compute_2d(abs_data->input, x, z, seed);
    
    // Trả về trị tuyệt đối của kết quả
    return abs(value);
}

/**
 * Duyệt qua cây Node (Visitor Pattern) tương đương với mapAll trong Abs.java
 */
static inline Noise abs_map_all(Noise noise, NoiseVisitorFunction visitor, void* user_data)
{
    if (visitor == NULL) return noise;

    AbsData* abs_data = (AbsData*)noise.data.custom_data;
    if (abs_data->input != NULL)
    {
        // Áp dụng bộ lọc và duyệt sâu xuống node input trước bằng hàm noise_map_all có sẵn
        Noise mapped_input = noise_map_all(*(abs_data->input), visitor, user_data);
        
        // Cập nhật lại cấu trúc dữ liệu con trỏ
        abs_data->input = &mapped_input; 
    }

    // Áp dụng visitor lên chính bản thân node Abs này
    Noise* result = visitor(&noise, user_data);
    return (result != NULL) ? *result : noise;
}


#endif
