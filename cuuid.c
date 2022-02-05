#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cuuid.h"
#include "pcg_basic.h"

static int rng_initialised = 0;

void rng_initialise() {
    uint64_t state = time(NULL) ^ (intptr_t)&time;
    uint64_t stream = (intptr_t)&rng_initialised;
    pcg32_srandom(state, stream);
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

// https://tools.ietf.org/html/rfc4122#section-3
void uuid_string(const uuid_t * uuid, uuid_str_t str) {
    static const char * s_hex = "0123456789abcdef";

    size_t chunk_bytes[5] = {4, 2, 2, 2, 6};
    size_t srcp = 0, dstp = 0;

    for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < chunk_bytes[i]; j++) {
            unsigned char byte = uuid->octet[srcp++];
            str[dstp++] = s_hex[byte >> 4];
            str[dstp++] = s_hex[byte & 0x0f];
        }
        str[dstp++] = '-';
    }
    
    str[dstp - 1] = '\0';
}
