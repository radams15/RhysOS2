//
// Created by rhys on 22/11/22.
//

#ifndef RHYSOS_MEMORY_H
#define RHYSOS_MEMORY_H

#include "Types.h"

void memset(uint8 *dest, uint8 val, uint32 len);
void memset_s(int8 *dest, int8 val, uint32 len);
void memcpy(uint8 *dest, uint8* src, uint32 len);

#endif //RHYSOS_MEMORY_H
