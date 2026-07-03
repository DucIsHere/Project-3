#ifndef NOISE_UTIL_C
#define NOISE_UTIL_C

#include "include/math/NoiseUtil.h"

#include <math.h>

Vec2f CELL_2D[256];
Vec2f GRAD_2D[256];
float SIN[SIN_COUNT];

/**
 * HÀM TIẾP TẾ: Nhận dữ liệu duỗi phẳng float[] từ Java gửi xuống
 * Cấu trúc mảng phẳng: [x0, y0, x1, y1, x2, y2...]
 */
void bind_native_noise_constants(float* java_cell_2d, float* java_grad_2d)
{
    // 2. KHỞI TẠO BẢNG SIN LƯỢNG TỬ (KHỚP ĐẾT LOGIC KHỞI TẠO TĨNH BÊN JAVA)
    // Vòng lặp thứ nhất: Tạo dải Sin mượt gốc
    for (int i = 0; i < SIN_COUNT; ++i) 
    {
        // Sử dụng PI2 = 6.2831855f nguyên bản
        SIN[i] = (float)sin(((double)i + 0.5) / SIN_COUNT * PI2);
    }

    // Vòng lặp thứ hai: Ép các mốc góc vuông (90, 180, 270) về giá trị Sin chuẩn
    // Khớp hoàn toàn dải sửa sai số góc của Java lúc init
    float degToIndex = SIN_COUNT / 360.0f;
    for (int i = 0; i < 360; i += 90) 
    {
        int index = (int)(i * degToIndex) & SIN_MASK;
        SIN[index] = (float)sin(i * 3.141592653589793 / 180.0);
    }

    // 3. COPY MẢNG SỐ NGẪU NHIÊN VÀ VECTOR HƯỚNG TỪ JAVA XUỐNG
    for (int i = 0; i < 256; i++)
    {
        // Ánh xạ mảng CELL_2D
        CELL_2D[i].x = java_cell_2d[i * 2];
        CELL_2D[i].y = java_cell_2d[i * 2 + 1];

        // Ánh xạ mảng GRAD_2D
        GRAD_2D[i].x = java_grad_2d[i * 2];
        GRAD_2D[i].y = java_grad_2d[i * 2 + 1];
    }
}

#endif
