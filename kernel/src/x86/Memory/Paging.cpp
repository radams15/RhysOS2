//
// Created by rhys on 23/11/22.
//

#include "Memory/Memory.h"
#include "Memory/Paging.h"
#include "Panic.h"
#include "Memory/malloc.h"

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

    for(int i=0 ; i< Memory::getPlacementAddress() ; i += 0x1000){
        PageFrame::alloc(getPage(i, 1, kernelDirectory), 0, 0);
    }

    ISR::register_handler(14, pageFaultHandler);

    switchDirectory(kernelDirectory);
}

void Paging::switchDirectory(PageDirectory *newDir) {

}

Page* Paging::getPage(unsigned int addr, int create, PageDirectory *directory) {

}

void Paging::fault(Registers r) {

}


uint32 *PageFrame::frames = NULL;
uint32 PageFrame::length = 0;

void PageFrame::init() {
    uint32 endPage = 0x1000000;

    length = endPage / 0x1000;
    frames = (uint32 *) Memory::kmalloc(indexFromBit(length));

    memset((uint8 *) frames, 0, indexFromBit(length));
}

void PageFrame::set(unsigned int addr) {
    uint32 frame = addr / 0x1000;
    uint32 idx = indexFromBit(frame);
    uint32 off = offsetFromBit(frame);

    frames[idx] |= (0x1 << off);
}

void PageFrame::clear(unsigned int addr) {
    uint32 frame = addr / 0x1000;
    uint32 idx = indexFromBit(frame);
    uint32 off = offsetFromBit(frame);
    frames[idx] &= ~(0x1 << off);
}

uint32 PageFrame::test(unsigned int addr) {
    uint32 frame = addr / 0x1000;
    uint32 idx = indexFromBit(frame);
    uint32 off = offsetFromBit(frame);
    return (frames[idx] & (0x1 << off));
}

uint32 PageFrame::first() {
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

    uint32 idx = first();

    if (idx == -1) {
        PANIC("No free frames!");
    }

    set(idx * 0x1000);
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
