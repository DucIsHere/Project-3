#include "CellFunction.h"

void cell_function_apply_batch(
    uint8_t type,
    int32_t seed,
    const int32_t* xc_arr,
    const int32_t* yc_arr,
    const float* dist_arr,
    const Vec2f* vec_arr,
    const Noise* lookup,
    float* out_results,
    int32_t count
) 
{
    CellFunctionType func = (CellFunctionType)func;

    // Vòng lặp quét mảng Off-Heap Memory do Java gửi xuống
    for (int32_t i = 0; i < count; i++) 
    {
        out_results[i] = cell_func_apply(
            func_type,
            seed,
            xc_arr[i],
            yc_arr[i],
            dist_arr[i],
            vec_arr[i],
            lookup
        );
    }
}