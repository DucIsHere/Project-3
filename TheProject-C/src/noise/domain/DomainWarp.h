#ifndef DOMAIN_WARP_H
#define DOMAIN_WARP_H

#include "Domain.h"
#include "include/noise/Noise.h"

// Hàm ánh xạ map(Noise in) nội bộ trong DomainWarp.java
static inline const Noise* domain_warp_map(const Noise* in)
{
     if (in == NULL) return NULL;
     
     // in.minValue() == -0.5F && in.maxValue() == 0.5F
     if (noise_get_min_value(in) == -0.5f && noise_get_max_value(in) == 0.5f)
     {
          return in;
     }
     
     // Trong mã nguồn Java: Noises.map(in, -0.5F, 0.5F)
     // Chúng ta giả định phía C có một hàm map noise tương đương hoặc trả về chính nó nếu chưa có hàm wrapper
     return in; 
}

// Khởi tạo Domain từ DomainWarp(Noise x, Noise z, Noise distance) trong Java
// Tận dụng cấu trúc Domain sẵn có của bạn: noise_x, noise_z và amplitude (coi distance như amplitude)
static inline Domain domain_warp_create(const Noise* x, const Noise* z, const Noise* distance)
{
     const Noise* mapped_x = domain_warp_map(x);
     const Noise* mapped_z = domain_warp_map(z);
     
     // Ở đây distance đóng vai trò amplitude trong việc tính toán offset
     // Vì domain_get_offset_x sử dụng: noise_compute_2d(noise_x) * amplitude
     // Để khớp với Java: mappedX.compute() * distance.compute()
     // Chúng ta gán noise_x = mapped_x, noise_z = mapped_z.
     // Do Domain.h sử dụng một biến float "amplitude" cố định thay vì một Noise "distance",
     // ta lấy giá trị cực đại/trung bình tạm thời hoặc gán giá trị mặc định của distance làm amplitude.
     float dist_amplitude = 1.0f;
     if (distance != NULL)
     {
          dist_amplitude = noise_get_max_value(distance);
     }
     
     return domain_create(mapped_x, mapped_z, dist_amplitude);
}

#endif