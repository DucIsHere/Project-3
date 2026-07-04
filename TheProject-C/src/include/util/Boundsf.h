#ifndef BOUNDSF_H
#define BOUNDSF_H

#include <stdbool.h>
#include <float.h>

typedef struct 
{
     float min_x;
     float min_y;
     float max_x;
     float max_y;
} Boundsf;

static const Boundsf BOUNDSF_NONE = { 1.0f, 1.0f, -1.0f, -1.0f };

static inline bool boundsf_contains(Boundsf* boundsf, float x, float y)
{
     return x >= boundsf->min_x && x <= boundsf->max_x && y >= boundsf->min_y && y <= boundsf->max_y;
}

typedef struct 
{
     float min_x;
     float min_y;
     float max_x;
     float max_y;
} BoundsfBuilder;

static inline void bound_builder_init(BoundsfBuilder* builder)
{
     builder->min_x = FLT_MAX; // Tương đương Float.MAX_VALUE
     builder->min_y = FLT_MAX;
     builder->max_x = -FLT_MAX; // Dùng âm FLT_MAX tương đương giá trị nhỏ nhất
     builder->max_y = -FLT_MAX;
}

static inline void bounds_builder_record(BoundsfBuilder* builder, float x, float y)
{
     if (x < builder->min_x) builder->min_x = x;
     if (y < builder->min_y) builder->min_y = y;
     if (x > builder->max_x) builder->max_x = x;
     if (y > builder->max_y) builder->max_y = y;
}

static inline Boundsf bounds_builder_build(const BoundsfBuilder* builder)
{
     Boundsf b = { builder->min_x, builder->min_y, builder->max_x, builder->max_y };
     return b;
}

#endif
