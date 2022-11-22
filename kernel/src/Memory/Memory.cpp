//
// Created by rhys on 22/11/22.
//

#include "Memory/Memory.h"

void memset(uint8 *dest, uint8 val, uint32 len) {
    uint8 *temp = (uint8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}