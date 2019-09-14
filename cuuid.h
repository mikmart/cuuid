#ifndef CUUID_H
#define CUUID_H

#ifdef __cplusplus
extern "C" {
#endif

struct uuid_s {
    unsigned char octet[16];
} typedef uuid_t;

// uuid.c
uuid_t uuid4_generate();
const char * uuid_string(const uuid_t * uuid);

#ifdef __cplusplus
} // extern C
#endif

#endif
