//
// Created by rhys on 23/11/22.
//

#include "Memory/malloc.h"
#include "Memory/Heap.h"
#include "Memory/Paging.h"

extern "C" uint32 end;

uint32 Memory::placementAddress = (uint32) &end;

uint32 Memory::kmalloc(uint32 size) {
    return kmalloc(size, FALSE, NULL);
}

uint32 Memory::kmalloc(uint32 size, bool align) {
    return kmalloc(size, align, NULL);
}

uint32 Memory::kmalloc(uint32 size, bool align, uint32* physical) {
    if(Heap::getKheap() != NULL){
        void* addr = Heap::getKheap()->alloc(size, align);
        if(physical != 0){
            Page* page = Paging::getPage((uint32) addr, FALSE, Paging::getKernelDirectory());
            *physical = page->frame*PAGE_SIZE + ((uint32)addr&0xFFF);
        }

        return (uint32) addr;
    }else {
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
}

uint32 Memory::getPlacementAddress() {
    return placementAddress;
}

void Memory::kfree(uint32 ptr) {
    return Heap::getKheap()->free((void*) ptr);
}

void *operator new(long unsigned int size) {
    return (void*) Memory::kmalloc(size);
}

void *operator new[](long unsigned int size) {
    return (void*) Memory::kmalloc(size);
}

void operator delete(void *p, long unsigned int n) {
    Memory::kfree((uint32) p);
}

void operator delete[](void *p, long unsigned int n) {
    Memory::kfree((uint32) p);
}

void operator delete(void *p) {
    Memory::kfree((uint32) p);
}

void operator delete[](void *p) {
    Memory::kfree((uint32) p);
}

extern "C" void __cxa_pure_virtual() {
    // Do nothing or print an error message.
}

extern "C" void __pure_virtual() {
    // Do nothing or print an error message.
}