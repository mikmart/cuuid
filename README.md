# cuuid

A minimal C library for generating version 4 universally unique identifiers (UUID4) specified in [RFC 4122](https://tools.ietf.org/html/rfc4122).

## Usage

Generate a UUID byte array with `uuid4_generate()`. To get a string, pass its pointer to `uuid_string()`:

``` c
#include <stdio.h>
#include "cuuid.h"

int main() {
    uuid_t uuid = uuid4_generate();
    printf("%s\n", uuid_string(&uuid));
}
```
