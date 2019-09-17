#ifndef CUUID_H
#define CUUID_H

#ifdef __cplusplus
extern "C" {
#endif

struct uuid_s {
    unsigned char octet[16];
} typedef uuid_t;

typedef char uuid_str_t[37];
typedef char uuid_hex_t[33];

uuid_t uuid4_generate();
void uuid_string(const uuid_t * uuid, uuid_str_t str);

#ifdef __cplusplus
} // extern C
#endif

#endif
