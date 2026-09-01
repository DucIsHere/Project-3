#prgama once

#if defined(__GNUC__) || defined(__clang__)
#define ECORE_INLINE inline __attribute__((always_inline))
#define ECORE_RESTRICT __restrict__
#else
#define ECORE_INLINE inline 
#define ECORE_RESTRICT __restrict__
#endif

typedef struct alignas(CACHE_LINE) ECoreFFIContext
{
     void* java_raw_in;
     void* java_raw_out;
     void* pcore_aligned_in;
     void* pcore_aligned_out;
     size_t element_count;
     size_t element_size;
     _Atomic(uint32_t) status;
} ECoreFFIContext;

// Khởi tạo ngữ cảnh FFI Bridge từ Java MemorySegment (Off-heap)
[[nodiscard]] ECoreFFIContext* ecore_ffi_create_context(void* java_in, void* java_out, size_t count, size_t elem_size);

// Giải phóng ngữ cảnh FFI Bridge
void ecore_ffi_destroy_context(ECoreFFIContext* ctx);

// Ép Thread Affinity cho E-Core (Các luồng từ 16 đến 23)
void ecore_set_thread_affinity(size_t thread_id);

// Tạo một Task cho E-Core chịu trách nhiệm Marshalling dữ liệu
[[nodiscard]] TaskHandle* ecore_create_prepare_task(Thrd* pool, ECoreFFIContext* ctx, TaskPriority prio);

// Tạo một Task cho E-Core chịu trách nhiệm Unpack dữ liệu trả về Java FFI
[[nodiscard]] TaskHandle* ecore_create_finish_task(Thrd* pool, ECoreFFIContext* ctx, TaskPriority prio);

// Trực tiếp căn chỉnh bộ nhớ 64-byte trên RAM mà không tốn chi phí Copy nếu bộ nhớ đã chuẩn
ECORE_INLINE static void* ecore_align_pointer(void* raw_ptr, size_t alignment) {
    uintptr_t addr = (uintptr_t)raw_ptr;
    uintptr_t aligned = (addr + (alignment - 1)) & ~(alignment - 1);
    return (void*)aligned;
}