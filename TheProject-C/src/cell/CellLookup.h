#ifndef CELL_LOOKUP_H
#define CELL_LOOKUP_H

#include <stdint.h>

typedef int (*CellLookupFn)(int32_t blockX, int32_t blockZ, void* userData);

// Tối ưu tính Index trong Chunk 16x16 (Dùng Bitwise Shift & AND né phép nhân)
static inline int cell_lookup_index_chunk16(int32_t localX, int32_t localZ) {
    return (localZ << 4) | (localX & 0x0F); // localZ * 16 + localX
}

// Tính Index tổng quát trên Grid kích thước bất kỳ (stride = width)
static inline int cell_lookup_index_grid(int32_t localX, int32_t localZ, int32_t stride) {
    return localZ * stride + localX;
}

#endif
