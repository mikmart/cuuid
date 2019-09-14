#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "cuuid.h"
#include "pcg_basic.h"

static int rng_initialized = 0;

void rng_initialize() {
    pcg32_srandom(time(NULL) ^ (intptr_t)&time, (intptr_t)&rng_initialized);
    rng_initialized = 1;
}

void random_bytes(void * buffer, size_t n) {
    if (!rng_initialized) rng_initialize();

    size_t n_ints = ceil(n / sizeof(uint32_t));
    uint32_t * bytes = malloc(sizeof(uint32_t) * n_ints);
    for (size_t i = 0; i < n_ints; i++) {
        bytes[i] = pcg32_random();
    }

    memcpy(buffer, bytes, n);
}

// https://tools.ietf.org/html/rfc4122#section-4.4
uuid_t uuid4_generate() {
    uuid_t uuid;

    random_bytes(&uuid, sizeof(uuid_t));

    // version bits: 0100 ----
    uuid.octet[6] &= 0x0f;
    uuid.octet[6] |= 0x01 << 6;
 
    // variant bits: 10-- ----
    uuid.octet[8] &= 0x3f;
    uuid.octet[8] |= 0x01 << 7;
    
    return uuid;
}

const char * bytes_to_hex(const void * buffer, size_t n) {
    static const char * hex = "0123456789abcdef";
    char * buf = (char *)buffer;

    char * str = malloc(n * 2 + 1);
    if (!str) return "";

    for (size_t i = 0; i < n; i++) {
        str[i * 2 + 0] = hex[(buf[i] >> 4) & 0x0f];
        str[i * 2 + 1] = hex[(buf[i] >> 0) & 0x0f];
    }

    str[n * 2] = 0;
    return str;
}

// https://tools.ietf.org/html/rfc4122#section-3
const char * uuid_string(const uuid_t * uuid) {
    const char * hex = bytes_to_hex(uuid, sizeof(uuid_t));
    if (!strlen(hex)) return ""; // malloc failed above

    size_t sizes[5] = {8, 4, 4, 4, 12};
    size_t dstp = 0, srcp = 0;

    char * str = malloc(sizeof(uuid_t) * 2 + 5);
    if (!str) return "";

    for (size_t i = 0; i < 5; i++) {
        memcpy(str + dstp, hex + srcp, sizes[i]);
        dstp += sizes[i];
        srcp += sizes[i];
        str[dstp++] = '-';
    }

    str[sizeof(uuid_t) * 2 + 4] = 0;
    return str;
}
