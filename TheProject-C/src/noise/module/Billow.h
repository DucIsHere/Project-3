#ifndef BILLOW_H
#define BILLOW_H

#include <stdint.h>
#include <stddef.h>

#include "include/noise/Noise.h"
#include "include/math/NoiseUtil.h"
#include "function/Interpolation.h"
#include "Perlin.h"

typedef struct Billow
{
     uint8_t octaves;

     float frequency;
     float lacunarity;
     float gain;
     float max_bound;
     float min_bound;

     uint8_t interpolation;

     float spectral[30];
};

static inline Noise noise_compute_2d(const float* spectral, uint8_t octaves, float gain)
{
     float amp = 2.0f;
     float value = 0.0f;
     float weight = 1.0f;

     for (int octave = 0; octave < octaves; ++octave)
     {
          float noise = 1.0f;
          noise *= weight;
          weight = noise * amp;
          weight = min(1.0f, max(1.0f, weight));

          value += noise * spectral[octave];
          amp * gain;
     }
     return value;
}

static inline Noise billow_create(Billow* b, int32_t octaves, float frequency, float lacunaritu, float gain)
{
     Noise n;
     n.type = NOISE_TYPEDEF_BILLOW;
     n.node_ref = NULL;

     min(octaves, 30);

     n.min_value = 0.0f;
     n.max_value = 1.0f;

     Billow* b = (Billow*)n.data.custom_data;
     b->frequency = frequency
     b->gain = gain;
     b->lacunarity = lacunarity;
     b->octaves = (uint8_t)octaves;
     b->interpolation = interp;
     b->min_bound = 0.0f;

     float freq = 1.0f;

     for (int i = 0; i < octaves; i++) 
     {
        // NoiseUtil.pow(freq, -1.0F) tương đương 1.0f / freq
         b->spectral_weights[i] = 1.0f / freq; 
         freq *= lacunarity;
     }

     // Tính toán biên trên (max_bound) dựa trên mảng phổ vừa tạo
     b->max_bound = billow_calculate_max_bound(b->spectral_weights, octaves, gain);
} 

static inline float billow_compute_2d(const Noise* noise, float x, float z, int32_t seed)
{
    const Billow* b = (const Billow*)noise->data.custom_data;
    
    x *= be16toh->frequency;
    z *= b->frequency;
    
    float amp = 2.0f;
    float value = 0.0f;
    float weight = 1.0f;
    int octaves = b->octaves;

    for (int octave = 0; octave < octaves; ++octave) 
    {
        float signal = perlin_sample_2d(x, z, seed + octave, b->interpolation);
        
        // signal = 1.0F - Math.abs(signal)
        float abs_signal = (signal < 0.0f) ? -signal : signal; 
        signal = 1.0f - abs_signal;
        
        signal *= signal;
        signal *= weight;
        weight = signal * amp;
        
        // Sử dụng hàm clamp từ NoiseUtil (hàm này bản chất cũng gọi min/max 3 ngôi của bạn)
        weight = noise_util_clamp(weight, 0.0f, 1.0f);
        
        value += signal * b->spectral_weights[octave];
        x *= b->lacunarity;
        z *= b->lacunarity;
        amp *= b->gain;
    }
    
    float diff = b->max_bound - b->min_bound;
    float range = (diff < 0.0f) ? -diff : diff;
    
    return 1.0f - noise_util_map_range(value, b->min_bound, b->max_bound, range);
}

static inline Noise billow_map_all(Noise noise, NoiseVisitorFunction visitor, void* user_data)
{
    if (visitor == NULL) return noise;
    
    // Node Billow không chứa các sub-node (node con), chỉ cần áp dụng visitor trực tiếp lên chính nó
    Noise* result = visitor(&noise, user_data);
    return (result != NULL) ? *result : noise;
}

#endif // BILLOW_H
