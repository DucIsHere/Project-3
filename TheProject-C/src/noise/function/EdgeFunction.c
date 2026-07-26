#include "EdgeFunction.h"

#include "immintrin.h"

__attribute__((visibility("default")))
void edge_function_apply_batch_avx2(
    EdgeFunctionType type,
    const float* __restrict dist1,
    const float* __restrict dist2,
    float* __restrict out_results,
    int count
) 
{
    // Vector 1.0f làm hằng số (- 1.0f)
    __m256 v_one = _mm256_set1_ps(1.0f);

    int i = 0;
    // Vòng lặp AVX2: Xử lý 8 phần tử float cùng 1 lúc
    for (; i <= count - 8; i += 8) {
        __m256 v_d1 = _mm256_loadu_ps(&dist1[i]);
        __m256 v_d2 = _mm256_loadu_ps(&dist2[i]);
        __m256 v_res;

        switch (type) 
        {
            case EDGE_FUNC_DISTANCE_2:
                // distance2 - 1.0f
                v_res = _mm256_sub_ps(v_d2, v_one);
                break;

            case EDGE_FUNC_DISTANCE_2_ADD:
                // distance2 + distance - 1.0f (Dùng FMA: d2 + d1 - 1.0)
                v_res = _mm256_sub_ps(_mm256_add_ps(v_d2, v_d1), v_one);
                break;

            case EDGE_FUNC_DISTANCE_2_SUB:
                // distance2 - distance - 1.0f
                v_res = _mm256_sub_ps(_mm256_sub_ps(v_d2, v_d1), v_one);
                break;

            case EDGE_FUNC_DISTANCE_2_MUL:
                // distance2 * distance - 1.0f (Sử dụng FMA _mm256_fnmadd_ps hoặc _mm256_fmadd_ps)
                v_res = _mm256_sub_ps(_mm256_mul_ps(v_d2, v_d1), v_one);
                break;

            case EDGE_FUNC_DISTANCE_2_DIV:
                // distance / distance2 - 1.0f
                v_res = _mm256_sub_ps(_mm256_div_ps(v_d1, v_d2), v_one);
                break;

            default:
                v_res = _mm256_setzero_ps();
                break;
        }

        // Lưu kết quả ra RAM (MemorySegment)
        _mm256_storeu_ps(&out_results[i], v_res);
    }

    // Xử lý các phần tử dư còn lại (Scalar fallback)
    for (; i < count; i++) 
    {
        out_results[i] = edge_func_apply(type, dist1[i], dist2[i]);
    }
}