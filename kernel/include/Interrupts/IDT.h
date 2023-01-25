//
// Created by rhys on 22/11/22.
//

#ifndef RHYSOS_IDT_H
#define RHYSOS_IDT_H

#include "Types.h"

struct IdtEntry {
    uint16 base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
    uint16 sel;                 // Kernel segment selector.
    uint8  always0;             // This must always be zero.
    uint8  flags;               // More flags. See documentation.
    uint16 base_hi;             // The upper 16 bits of the address to jump to.
} __attribute__((packed));

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct IdtPtr {
    uint16 limit;
    uint32 base;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed));


class IDT {
private:
    static IdtEntry entries[256];
    static IdtPtr ptr;

public:
    static void init();

    static void setGate(uint8 num, uint32 base, uint16 sel, uint8 flags);

    static void flush(uint32 ptr);

    static void enable();
    static void disable();
};

extern "C" void  isr0();
extern "C" void  isr1();
extern "C" void  isr2();
extern "C" void  isr3();
extern "C" void  isr4();
extern "C" void  isr5();
extern "C" void  isr6();
extern "C" void  isr7();
extern "C" void  isr8();
extern "C" void  isr9();
extern "C" void  isr10();
extern "C" void  isr11();
extern "C" void  isr12();
extern "C" void  isr13();
extern "C" void  isr14();
extern "C" void  isr15();
extern "C" void  isr16();
extern "C" void  isr17();
extern "C" void  isr18();
extern "C" void  isr19();
extern "C" void  isr20();
extern "C" void  isr21();
extern "C" void  isr22();
extern "C" void  isr23();
extern "C" void  isr24();
extern "C" void  isr25();
extern "C" void  isr26();
extern "C" void  isr27();
extern "C" void  isr28();
extern "C" void  isr29();
extern "C" void  isr30();
extern "C" void  isr31();
extern "C" void  isr128();

extern "C" void  irq0();
extern "C" void  irq1();
extern "C" void  irq2();
extern "C" void  irq3();
extern "C" void  irq4();
extern "C" void  irq5();
extern "C" void  irq6();
extern "C" void  irq7();
extern "C" void  irq8();
extern "C" void  irq9();
extern "C" void  irq10();
extern "C" void  irq11();
extern "C" void  irq12();
extern "C" void  irq13();
extern "C" void  irq14();
extern "C" void  irq15();

#endif //RHYSOS_IDT_H
