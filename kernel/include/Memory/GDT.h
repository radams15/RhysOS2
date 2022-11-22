//
// Created by rhys on 22/11/22.
//

#ifndef RHYSOS_GDT_H
#define RHYSOS_GDT_H

#include "Types.h"

struct GdtEntry {
    uint16 limitLow;           // The lower 16 bits of the limit.
    uint16 baseLow;            // The lower 16 bits of the base.
    uint8  baseMiddle;         // The next 8 bits of the base.
    uint8  access;              // Access flags, determine what ring this segment can be used in.
    uint8  granularity;
    uint8  baseHigh;           // The last 8 bits of the base.
} __attribute__((packed));

struct GdtPtr {
    uint16 limit;
    uint32 base;
} __attribute__((packed));

class GDT {
private:

    static GdtEntry entries[5];
    static GdtPtr ptr;

public:
    static void init();

    static void setGate(int32 num, uint32 base, uint32 limit, uint8 access, uint8 granularity);

    static void flush(uint32 toFlush);
};


#endif //RHYSOS_GDT_H
