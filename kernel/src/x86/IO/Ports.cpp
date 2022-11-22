//
// Created by rhys on 19/11/22.
//

#include "IO/Ports.h"

extern "C" void port_outb(uint16 port, uint8 value);

void Ports::outb(uint16 port, uint8 value) {
    asm volatile("outb %1, %0" :: "dN"(port), "a"(value));
    //port_outb(port, value);
}

uint8 Ports::inb(uint16 port) {
    uint8 out;

    asm volatile(
            "inb %1, %0"
            : "=a"(out)
            : "Nd"(port)
            );

    return out;
}

uint16 Ports::inw(uint16 port) {
    uint16 out;

    asm volatile(
            "in %%dx, %%ax"
            : "=a"(out)
            : "d"(port)
            );

    return out;
}
