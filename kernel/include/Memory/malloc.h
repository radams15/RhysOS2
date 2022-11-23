//
// Created by rhys on 23/11/22.
//

#ifndef RHYSOS_MALLOC_H
#define RHYSOS_MALLOC_H

#include "Types.h"

class Memory {
private:

    static uint32 placementAddress;

public:
    static uint32 kmalloc(uint32 size);

    static uint32 kmalloc(uint32 size, bool align);

    static uint32 kmalloc(uint32 size, bool align, uint32 *physical);

    static uint32 getPlacementAddress();
};

#endif //RHYSOS_MALLOC_H
