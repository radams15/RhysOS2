//
// Created by rhys on 23/11/22.
//

#include "Memory/Memory.h"
#include "Memory/Paging.h"
#include "Panic.h"
#include "Memory/malloc.h"
#include "Memory/Heap.h"

PageDirectory *Paging::kernelDirectory = NULL;
PageDirectory *Paging::currentDirectory = NULL;

extern "C" void pageFaultHandler(Registers registers){
    Paging::fault(registers);
}

void Paging::init() {
    PageFrame::init();

    kernelDirectory = (PageDirectory *) Memory::kmalloc(sizeof(PageDirectory), TRUE);
    memset((uint8 *) kernelDirectory, 0, sizeof(PageDirectory));
    currentDirectory = kernelDirectory;

    for(uint32 i=Heap::KHEAP_START ; i<Heap::KHEAP_START+Heap::KHEAP_INITIAL_SIZE ; i+=PAGE_SIZE){
        getPage(i, TRUE, kernelDirectory);
    }

    for(uint32 i=0 ; i<Memory::getPlacementAddress()+PAGE_SIZE ; i += PAGE_SIZE){
        PageFrame::alloc(getPage(i, TRUE, kernelDirectory), FALSE, FALSE);
    }

    ISR::register_handler(14, pageFaultHandler);

    switchDirectory(kernelDirectory);

    Heap::setKheap(new Heap((uint32)Heap::KHEAP_START, (uint32) Heap::KHEAP_START+Heap::KHEAP_INITIAL_SIZE, (uint32) 0xCFFFF000, FALSE, FALSE));
}

void PageFrame::init() {
    uint32 endPage = 0x1000000;

    length = endPage / PAGE_SIZE;
    frames = (uint32 *) Memory::kmalloc(indexFromBit(length));

    memset((uint8 *) frames, 0, indexFromBit(length));
}

void Paging::switchDirectory(PageDirectory *newDir) {
    currentDirectory = newDir;

    asm volatile("mov %0, %%cr3" :: "r"(&currentDirectory->physicalTables)); // Pass physical addr of page table.

    uint32 cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Set paging bit.
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

Page* Paging::getPage(unsigned int addr, int create, PageDirectory *directory) {
    addr /= PAGE_SIZE; // Addr => index

    uint32 idx = addr/1024; // Location of page table with the index.

    if(directory->tables[idx]){
        return & directory[idx].tables[idx]->pages[addr%1024];
    }

    if(create){
        uint32 temp;

        directory->tables[idx] = (PageTable*) Memory::kmalloc(sizeof(PageTable), true, &temp);
        memset((uint8*) directory->tables[idx], 0, PAGE_SIZE);
        directory->physicalTables[idx] = temp|0x7; // Set present, rw and us

        return & directory[idx].tables[idx]->pages[addr%1024];
    }

    return NULL;
}

void Paging::fault(Registers r) {
    uint32 faultAddr;
    asm volatile("mov %%cr2, %0" : "=r"(faultAddr));

    TTY::printk("Page fault at %d:\n\tpresent? %d\n\tRO: %d\n\tUser mode: %d\n\tReserved: %d",
                faultAddr,
                !(r.err_code&0x1),
                r.err_code&0x2,
                r.err_code&0x4,
                r.err_code&0x8,
                r.err_code&0x10
    );

    halt();
}


uint32 *PageFrame::frames = NULL;
uint32 PageFrame::length = 0;

void PageFrame::set(unsigned int addr) {
    uint32 frame = addr / PAGE_SIZE;
    uint32 idx = indexFromBit(frame);
    uint32 off = offsetFromBit(frame);

    frames[idx] |= (0x1 << off);
}

void PageFrame::clear(unsigned int addr) {
    uint32 frame = addr / PAGE_SIZE;
    uint32 idx = indexFromBit(frame);
    uint32 off = offsetFromBit(frame);
    frames[idx] &= ~(0x1 << off);
}

uint32 PageFrame::test(unsigned int addr) {
    uint32 frame = addr / PAGE_SIZE;
    uint32 idx = indexFromBit(frame);
    uint32 off = offsetFromBit(frame);
    return (frames[idx] & (0x1 << off));
}

int32 PageFrame::first() {
    uint32 i, j;
    for (i = 0; i < indexFromBit(length); i++) {
        if (frames[i] != 0xFFFFFFFF) // nothing free, exit early.
        {
            // at least one bit is free here.
            for (j = 0; j < 32; j++) {
                uint32 toTest = 0x1 << j;
                if (!(frames[i] & toTest)) {
                    return i * 4 * 8 + j;
                }
            }
        }
    }

    return -1;
}


constexpr uint32 PageFrame::indexFromBit(uint32 a) {
    return (a / (8 * 4));
}

constexpr uint32 PageFrame::offsetFromBit(uint32 a) {
    return (a % (8 * 4));
}

void PageFrame::alloc(Page *page, bool isKernel, bool isWriteable) {
    if (page->frame != 0) {
        return;
    }

    int32 idx = first();

    if (idx == -1) {
        PANIC("No free frames!");
    }

    set(idx * PAGE_SIZE);
    page->present = TRUE;
    page->rw = isWriteable;
    page->user = !isKernel;
    page->frame = idx;
}

void PageFrame::free(Page *page) {
    if (page->frame == 0) {
        return;
    }

    clear(page->frame);
    page->frame = 0;
}
