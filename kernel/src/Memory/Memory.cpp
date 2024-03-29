//
// Created by rhys on 22/11/22.
//

#include "Memory/Memory.h"

void memset(uint8 *dest, uint8 val, uint32 len) {
    uint8 *temp = (uint8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

void memset_s(int8 *dest, int8 val, uint32 len) {
    int8 *temp = (int8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

void memcpy(uint8 *dest, uint8* src, uint32 len) {
    uint8 *new_src = (uint8 *)src;
    uint8 *new_dest = (uint8 *)dest;

    for ( ; len != 0; len--) *new_dest++ = *new_src++;
}

int8 memcmp(unsigned char *cs, unsigned char *ct, unsigned int n) {
    uint32 i;

    for (i = 0; i < n; i++, cs++, ct++)
    {
        if (*cs < *ct)
        {
            return -1;
        }
        else if (*cs > *ct)
        {
            return 1;
        }
    }

    return 0;
}
