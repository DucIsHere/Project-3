#ifndef ADD_H
#define ADD_H

#include <stdint.h>
#include <stddef.h>

#include "include/noise/Noise.h" // Nhận struct Noise

typedef struct
{
     const Noise* input1;
     const Noise* input2;
} AddData;

// Tính toán 2D đệ quy ngược lại Noise.h
static inline float add_compute_2d_internal(const AddData* ad, float x, float z, int32_t seed)
{
     if (ad == NULL) return 0.0f;
     
     // Gọi an toàn: Nếu node con NULL thì trả về 0, tránh crash pointer
     float offset1 = (ad->input1 != NULL) ? noise_compute_2d(ad->input1, x, z, seed) : 0.0f;
     float offset2 = (ad->input2 != NULL) ? noise_compute_2d(ad->input2, x, z, seed) : 0.0f;
     return offset1 + offset2;
}

// Factory khởi tạo gán thẳng vào Noise tổng
static inline Noise noises_add_create(const Noise* i1, const Noise* i2)
{
     Noise n;
     n.type = NOISE_TYPEDEF_ADD; // Giả sử loại 5 tương ứng với ADD node
     n.node_ref = NULL; // Không dùng FastNoise
     
     // Tính biên độ động dựa trên 2 node con
     n.min_value = (i1 ? i1->min_value : 0.0f) + (i2 ? i2->min_value : 0.0f);
     n.max_value = (i1 ? i1->max_value : 0.0f) + (i2 ? i2->max_value : 0.0f);
     
     // Ép kiểu vùng nhớ custom_data 32 bytes
     AddData* ad = (AddData*)n.data.custom_data;
     ad->input1 = i1;
     ad->input2 = i2;
     
     return n;
}

#endif