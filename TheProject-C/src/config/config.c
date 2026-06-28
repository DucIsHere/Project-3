#ifndef CONFIG_C
#define CONFIG_C

#define CACHE_LINE_SIZE 64
#define SYSTEM_ALIGNMENT __attribute__((aligned(CACHE_LINE_SIZE)))

#define CHUNK_WIDTH 16

#define MAX_CACHE_BATCH_COUNT 6

#endif