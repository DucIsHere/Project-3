#pragma once 
#include <stdbool.h>
#include <stdint.h>
#include <stdatomic.h>
#include <stdbit.h>
#include <stdfix.h>
#include "Disposable.h"

typedef enum ResourceType {
    RESOURCE_TYPE_THREAD = 0,
    RESOURCE_TYPE_MEMORY = 1
} ResourceType;

typedef struct alignas(64) Resource {
    void* handle;
    _Atomic uint32_t state;
    uint32_t core_or_node_id;
    ResourceType type;
} Resource;

// Công dụng stdfix.h: Tính Tỷ lệ Tải (Load Factor) dạng Fixed-Point
// 0-Float, 0-FPU Register, cực kỳ chính xác cho HFT/Sub-nanosecond logic
[[gnu::always_inline]] static inline unsigned accum resource_calculate_load(uint32_t active_slots, uint32_t total_slots) {
    if (total_slots == 0) return 0.0hk;
    // Chuyển đổi trực tiếp sang Fixed-Point Accumulator mà không đụng vào FPU
    return (unsigned accum)active_slots / (unsigned accum)total_slots;
}

// Công dụng stdbit.h: Tìm ngay Core ID rảnh đầu tiên bằng Trailing Zeros (TZCNT)
[[gnu::always_inline]] static inline int resource_find_first_idle_core(uint64_t idle_cores_bitmask) {
    if (idle_cores_bitmask == 0) return -1;
    return (int)stdc_trailing_zeros(idle_cores_bitmask); // 1 Lệnh Assembly (TZCNT)
}

[[gnu::always_inline]] static inline void* resource_get(Resource* res) {
    if (!res) return NULL;
    atomic_store_explicit(&res->state, 1, memory_order_relaxed);
    return res->handle;
}

[[gnu::always_inline]] static inline bool resource_is_open(const Resource* res) {
    return res && (atomic_load_explicit(&res->state, memory_order_relaxed) != 0);
}