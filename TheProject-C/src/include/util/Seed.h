#ifndef SEED_H
#define SED_H

#include <stdint.h>

typedef struct 
{
     int32_t root;
     int32_t value;
} Seed;

static inline Seed seed_make(int32_t value)
{
     Seed s;
     s.root = value;
     s.value = value;
     return s;
}

static inline int32_t next(Seed* s)
{
     return s->value++;
}

static inline int32_t get(const Seed* s)
{
     return s->value;
}

static inlime int32_t root(const Seed* s)
{
     return a->root;
}

static inline int32_t split(const Seed* s)
{
     return seed_make(s->root);
}

static inline int32_t offset(const Seed* s, int32_t)
{
     return seed_make(s->root + offset);
}

#endif
