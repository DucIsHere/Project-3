#include <atdlib.h>

#include "include/noise/Noise.h"

uint64_t native_create_noise(const char* encoded_str, float min_v, float max_v) 
{
    Noise* noise = (Noise*)malloc(sizeof(Noise));
    if (noise == NULL) return 0;
    
    *noise = noise_create(encoded_str, min_v, max_v);
    return (uint64_t)noise;
}

// Hàm giải phóng bộ nhớ do Java ra lệnh khi tắt thế giới
void native_free_noise(uint64_t noise_ptr) 
{
    Noise* noise = (Noise*)noise_ptr;
    if (noise != NULL) {
        noise_free(noise);
        free(noise);
    }
}