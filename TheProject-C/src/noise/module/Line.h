#ifndef LINE_H
#define LINE_H

#include <stdint.h>
#include <stdbool.h>

#include "include/noise/Noise.h"

typedef struct 
{
     float x1;
     float z1;
     float x2;
     float z2;
     float orth_x1;
     float orth_z1;
     float orth_x2;
     float orth_z2;

     float dx;
     float dz;
     float length_sq;

     float feather;
     float feather_bias;

     Noise radius_sq;
     Noise fade_in;
     Noise fade_out;

} LineNoise;

static inline Noise line(LineNoise* l, float x1, float z1, float x2, float z2, Noise radius_sq, Noise fade_in, Noise fade_out, float feather, float dx, float dz)
{
     Noise n;
     n.type = NOISE_TYPEDEF_LINE;
     n.node_ref = NULL;

     n.min_value = 0.0f;
     n.max_value = 1.0f;

     LineNoise* l = (LineNoise*)n.data.custom_data;
     l->x1 = x1;
     l->z1 = z1;
     l->x2 = x2;
     l->z2 = z2;

     l->dx = dx;
     l->dz = dz;

     l->radius_sq = radius_sq;
     l->fade_in = fade_in;
     l->fade_out = fade_out;

     l->feather = feather;
     l->feather_bias = 1.0f - feather;

     l->orth_x1 = x1 + (z2 - z1);
     l->orth_z1 = z1 + (x1 - x2);
     l->orth_x2 = x2 + (z2 - z1);
     l->orth_z2 = z2 + (x1 - x2);
    
     l->length_sq = l->dx * l->dx + l->dz * l->dz;

     return n;
}

static inline float distSq(float x1, float y1, float x2, float y2)
{
     float dx = x2 - x1;
     float dy = y2 - y1;
     return dx * dx + dy * dy;
}

static inline float distance_on_line(float x, float y, float ax, float ay, float bx, float by)
{
     float dx = bx - ax;
     float dy = by - ay;
     float v = (x - ax) * dx + (y - ay) * dy;
     return v / (dx * dx + dy * dy);
}

static inline float get_distance_sq(float x, float y)
{
     float t = (x - l->x1) * l->dx + (y - l->z1) * l->dz;
     float s = clamp(t / l->length_sq, 0.0f, 1.0f);
     float ix = l->x1 + s * l->dx;
     float iy = l->z1 + s * l->dz;
     return distSq(x, y, ix, iy);
}

static inline int32_t relativeCCW(float x1, float y1, float x2, float y2, float px, float py)
{
     x2 -= x1;
     y2 -= y1;
     px -= x1;
     py -= y1;

     double ccw = px * y2 - py * x2;

     if (ccw == 0)
     {
          ccw = px * x2 + py * y2;
          if (ccw > 0.0f)
          {
               px -= x2;
               py -= y2;
               ccw = px * x2 + py * y2;
               if (ccw < 0.0f)
               {
                    ccw = 0.0f;
               }
          }
     }
     return (ccw < 0.0) ? -1 : ((ccw > 0.0) ? 1 : 0);
}

static inline float get_width_modifier(float x, float z, int32_t seed)
{
     float d1 = distSq(x, z, l->x1, l->z1);
     if (d1 == 0.0f) return 0.0f;

     float d2 = distSq(x, z, l->x2, l->z2);
     if (d2 == 0.0f) return 0.0f;

     float fade = 1.0f;
     float in = noise_compute_2d(&l->fade_in, x, z, seed);
     float out = noise_compute_2d(&l->fade_out, x, z, seed);

     if (in > 0.0f)
     {
          float dist = in * l->length_sq;
          if (d1 < dist)
          {
               fade *= d1 / dist;
          }
     }

     if (out > 0.0f)
     {
          float dist = out * l->length_sq;
          if (d2 < dist)
          {
               fade *= d2 / dist;
          }
     }
     return fade;
}

static inline bool intersect(float ax1, float ay1, float ax2, float ay2, float bx1, float by1, float bx2, float by2)
{
     return relativeCCW(ax1, ay1, ax2, ay2, bx1, by1) * relativeCCW(ax1, ay1, ax2, ay2, bx2, by2) <= 0 && relativeCCW(bx1, by1, bx2, by2, ax1, ay1) * relativeCCW(bx1, by1, bx2, by2, ax2, ay2) <= 0;
}

static inline Noise noise_compute_2d(float x, float z, int32_t seed)
{
     float widthModifier = get_width_modifier(x, z, seed);
     float dist2 = get_distance_sq(x, z);
     float radius2 = noise_compute_2d(l->radius_sq, x, z, seed) * widthModifier;
     if (dist2 > radius2) { return 0.0f; }
     float value = dist2 / radius2;
     if (l->feather == 0.0f)
     {
          return 1.0f - value;
     }
     float feather = l->feather_bias + widthModifier * l->feather;
     return (1.0F - value) * l->feather;
}

#endif
