//
// Created by rhys on 23/11/22.
//

#ifndef RHYSOS_HEAP_H
#define RHYSOS_HEAP_H

#include "Types.h"
#include "libc/List.h"

struct HeapHeader {
    uint32 magic;
    uint8 isHole;
    uint32 size;
};

struct HeapFooter {
    uint32 magic;
    HeapHeader* header;
};

class Heap {
private:
    List<HeapHeader*> index;

    uint32 start;
    uint32 end;
    uint32 max;
    uint8 supervisor;
    uint8 readOnly;

    enum {
        KHEAP_START = 0xC0000000,
        KHEAP_INITIAL_SIZE = 0x100000,
        HEAP_INDEX_SIZE = 0x20000,
        HEAP_MAGIC = 0x123890AB,
        HEAP_MIN_SIZE = 0x70000
    };

    int32 smallestHole(uint32 size, uint8 align);

public:
    Heap(uint32 start, uint32 end, uint32 max, uint8 supervisor, uint8 readOnly);
    void* alloc();
    void* free(void* ptr);
};


#endif //RHYSOS_HEAP_H
