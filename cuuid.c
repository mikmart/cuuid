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

void uuid_hex(const uuid_t * uuid, uuid_hex_t str) {
    static const char * hex = "0123456789abcdef";
    for (size_t i = 0; i < sizeof(uuid_t); i++) {
        str[i * 2 + 0] = hex[uuid->octet[i] >> 4];
        str[i * 2 + 1] = hex[uuid->octet[i] & 0x0f];
    }
    str[sizeof(uuid_t) * 2] = 0;
}

// https://tools.ietf.org/html/rfc4122#section-3
void uuid_string(const uuid_t * uuid, uuid_str_t str) {
    uuid_hex_t hex;
    uuid_hex(uuid, hex);

    size_t sizes[5] = {8, 4, 4, 4, 12};
    size_t dstp = 0, srcp = 0;

    for (size_t i = 0; i < 5; i++) {
        memcpy(str + dstp, hex + srcp, sizes[i]);
        dstp += sizes[i];
        srcp += sizes[i];
        str[dstp++] = '-';
    }
    
    str[strlen(hex) + 4] = 0;
}
