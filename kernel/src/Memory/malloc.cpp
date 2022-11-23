//
// Created by rhys on 23/11/22.
//

#include "Memory/malloc.h"

extern "C" uint32 end;

uint32 Memory::placementAddress = (uint32) &end;

uint32 Memory::kmalloc(uint32 size) {
    return kmalloc(size, FALSE, NULL);
}

uint32 Memory::kmalloc(uint32 size, bool align) {
    return kmalloc(size, align, NULL);
}

uint32 Memory::kmalloc(uint32 size, bool align, uint32* physical) {
    if (align == 1 && (placementAddress & 0xFFFFF000)) { // If the address is not already page-aligned then align it
        placementAddress &= 0xFFFFF000;
        placementAddress += 0x1000;
    }

    if (physical) {
        *physical = placementAddress;
    }
    uint32 tmp = placementAddress;
    placementAddress += size;

    return tmp;
}

uint32 Memory::getPlacementAddress() {
    return placementAddress;
}
