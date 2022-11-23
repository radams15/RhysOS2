//
// Created by rhys on 23/11/22.
//

#include "Heap.h"
#include "Memory/Heap.h"

int8 heapHeaderComparator(HeapHeader* a, HeapHeader* b){
    return a->size < b->size;
}

Heap::Heap(uint32 start, uint32 end, uint32 max, uint8 supervisor, uint8 readOnly)
    : start(start), end(end), max(max), supervisor(supervisor), readOnly(readOnly),
    index((void*) start, HEAP_INDEX_SIZE, heapHeaderComparator) {

    start += sizeof(void*)*HEAP_INDEX_SIZE;

    // TODO not done
}

void *Heap::alloc() {

}

void *Heap::free(void* d) {

}

int32 Heap::smallestHole(uint32 size, uint8 align) {
    uint32 i = 0;

    while(i<index.length()){
        HeapHeader* head = index.get(i);

        if(align){
            uint32 loc = (uint32) head;
            int32 offset = 0;

            if ((loc+sizeof(HeapHeader)) & 0xFFFFF000 != 0) {
                offset = 0x1000 /* page size */  - (loc + sizeof(HeapHeader)) % 0x1000;
            }

            int32 holeSize = head->size-offset;

            if(holeSize >= (int32) size){
                break;
            }
        }else if(head->size >= size){
            break;
        }

        i++;
    }

    if(i == index.length()){
        return -1;
    }

    return i;
}
