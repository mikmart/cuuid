#ifndef CUUID_H
#define CUUID_H

#ifdef __cplusplus
extern "C" {
#endif

struct uuid_s {
    unsigned char octet[16];
} typedef uuid_t;

#define UUID_STRLEN 37

// uuid.c
uuid_t uuid4_generate();
void uuid_string(const uuid_t * uuid, char str[UUID_STRLEN]);

#ifdef __cplusplus
} // extern C
#endif

#endif
