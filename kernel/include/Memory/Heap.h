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

    int32 smallestHole(uint32 size, uint8 align);

    static Heap* kheap;

public:
    enum {
        KHEAP_START = 0xC0000000,
        KHEAP_INITIAL_SIZE = 0x100000,
        HEAP_INDEX_SIZE = 0x20000,
        HEAP_MAGIC = 0x123890AB,
        HEAP_MIN_SIZE = 0x70000
    };

    Heap(uint32 start, uint32 end, uint32 max, uint8 supervisor, uint8 readOnly);
    void* alloc(uint32 size, uint8 align);
    void free(void* p);

    void expand(uint32 newSize);
    uint32 contract(uint32 newSize);

    static Heap* getKheap();
    static void setKheap(Heap* newKheap);
};


#endif //RHYSOS_HEAP_H
