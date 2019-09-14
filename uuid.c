#include <stdlib.h>
#include <string.h>
#include "cuuid.h"

void random_bytes(void * buffer, size_t n) {
    // TODO: Use a better source of randomness
    for (size_t i = 0; i < n; i++) {
        ((char *)buffer)[i] = rand();
    }
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
