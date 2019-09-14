#include <string.h> // memcpy
#include <time.h>
#include "pcg_basic.h"

static int rng_initialized = 0;

void rng_initialize() {
    pcg32_srandom(time(NULL) ^ (intptr_t)&time, 5);
    rng_initialized = 1;
}

#define BATCH_SIZE sizeof(uint32_t)

void random_bytes(void * buffer, size_t n) {
    if (!rng_initialized) rng_initialize();
    size_t remaining = n;
    while (1) {
        uint32_t bytes = pcg32_random();
        if (remaining > BATCH_SIZE) {
            memcpy(buffer, &bytes, BATCH_SIZE);
            remaining -= BATCH_SIZE;
        } else {
            memcpy(buffer, &bytes, remaining);
            break;
        }
        buffer += BATCH_SIZE;
    }
}

#undef BATCH_SIZE
