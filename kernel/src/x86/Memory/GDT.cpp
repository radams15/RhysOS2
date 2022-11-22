//
// Created by rhys on 22/11/22.
//

#include "Memory/GDT.h"

extern "C" void gdt_flush(uint32 ptr);

GdtEntry GDT::entries[5];
GdtPtr GDT::ptr;

void GDT::init() {
    ptr.limit = (sizeof(GdtEntry) * 5) - 1;

    ptr.base  = (uint32) &entries;

    setGate(0, 0, 0, 0, 0);                // Null segment
    setGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    setGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    setGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    setGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    gdt_flush((uint32) &ptr);
}

void GDT::setGate(int32 num, uint32 base, uint32 limit, uint8 access, uint8 granularity) {
        entries[num].baseLow    = (base & 0xFFFF);
        entries[num].baseMiddle = (base >> 16) & 0xFF;
        entries[num].baseHigh   = (base >> 24) & 0xFF;

        entries[num].limitLow   = (limit & 0xFFFF);
        entries[num].granularity = (limit >> 16) & 0x0F;

        entries[num].granularity |= granularity & 0xF0;
        entries[num].access      = access;
}

void GDT::flush(uint32 toFlush) {
    gdt_flush(toFlush);
}
