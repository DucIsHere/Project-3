#ifndef DOMAINS_H
#define DOMAINS_H

#include "Domain.h"
#include "DomainWarp.h"
#include "DirectionWarp.h"
#include "include/noise/Noise.h"

// public static Domain domain(Noise x, Noise z, Noise distance)
static inline Domain domains_domain(const Noise* x, const Noise* z, const Noise* distance)
{
     return domain_warp_create(x, z, distance);
}

// public static Domain direction(Noise direction, Noise distance)
static inline Domain domains_direction(const Noise* direction, const Noise* distance)
{
     // Sử dụng DirectionWarp từ file DirectionWarp.java/DirectionWarp.h đã ánh xạ trước đó
     // direction tương ứng với direction, distance đóng vai trò strength / amplitude
     float strength_val = 1.0f;
     if (distance != NULL)
     {
          strength_val = noise_get_max_value(distance);
     }
     
     // Khởi tạo struct DirectionWarp dựa trên file DirectionWarp.java đầu tiên
     DirectionWarp dw = direction_warp_create(direction, distance);
     
     // Trả về một đối tượng Domain chuẩn tương thích với Domain.h của bạn
     return domain_create(dw.direction, dw.strength, strength_val);
}

// public static Domain direct()
static inline Domain domains_direct(void)
{
     // DirectWarp trả về domain không dịch chuyển (offset = 0)
     return domain_create(NULL, NULL, 0.0f);
}

#endif