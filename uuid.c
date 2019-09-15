#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cuuid.h"
#include "pcg_basic.h"

static int rng_initialised = 0;

void rng_initialise() {
    pcg32_srandom(time(NULL) ^ (intptr_t)&time, (intptr_t)&rng_initialised);
    rng_initialised = 1;
}

// https://tools.ietf.org/html/rfc4122#section-4.4
uuid_t uuid4_generate() {
    if (!rng_initialised) rng_initialise();

    uuid_t uuid;
    for (size_t i = 0; i < 4; i++) {
        ((uint32_t *)&uuid)[i] = pcg32_random();
    }
    
    // version bits: 0100 ----
    uuid.octet[6] &= 0x0f;
    uuid.octet[6] |= 0x01 << 6;
 
    // variant bits: 10-- ----
    uuid.octet[8] &= 0x3f;
    uuid.octet[8] |= 0x01 << 7;
    
    return uuid;
}

void uuid_hex(const uuid_t * uuid, char str[33]) {
    static const char * hex = "0123456789abcdef";
    for (size_t i = 0; i < 16; i++) {
        str[i * 2 + 0] = hex[(uuid->octet[i] >> 4) & 0x0f];
        str[i * 2 + 1] = hex[(uuid->octet[i] >> 0) & 0x0f];
    }
    str[32] = 0;
}

// https://tools.ietf.org/html/rfc4122#section-3
void uuid_string(const uuid_t * uuid, uuid_str_t str) {
    char hex[33];
    uuid_hex(uuid, hex);

    size_t sizes[5] = {8, 4, 4, 4, 12};
    size_t dstp = 0, srcp = 0;

    for (size_t i = 0; i < 5; i++) {
        memcpy(str + dstp, hex + srcp, sizes[i]);
        dstp += sizes[i];
        srcp += sizes[i];
        str[dstp++] = '-';
    }
    
    str[36] = 0;
}
