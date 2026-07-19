#include "lib/Matrix.h"

// ==========================================================
// EXPORT API: ĐÓNG GÓI HÀM KHỞI TẠO MA TRẬN CHO JAVA PANAMA
// ==========================================================

Mat2x1 c_mat2x1_set(float m0, float m1) {
    return mat2x1_set(m0, m1);
}

Mat2x2 c_mat2x2_set(float m00, float m01, float m10, float m11) {
    return mat2x2_set(m00, m01, m10, m11);
}

Mat2x3 c_mat2x3_set(float m00, float m01, float m10, float m11, float m21, float m22) {
    return mat2x3_set(m00, m01, m10, m11, m21, m22);
}

Mat3x1 c_mat3x1_set(float m00, float m01, float m10) {
    return mat3x1_set(m00, m01, m10);
}

Mat3x2 c_mat3x2_set(float m00, float m01, float m10, float m11, float m21, float m22) {
    return mat3x2_set(m00, m01, m10, m11, m21, m22);
}

Mat3x3 c_mat3x3_set(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) {
    return mat3x3_set(m00, m01, m02, m10, m11, m12, m20, m21, m22);
}
