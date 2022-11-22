//
// Created by rhys on 22/11/22.
//

#include "Interrupts/IDT.h"
#include "Memory/Memory.h"
#include "IO/Ports.h"
#include "Interrupts/ISR.h"

extern "C" void idt_flush(uint32 ptr);
IdtEntry IDT::entries[256];
IdtPtr IDT::ptr;

extern "C" void irq_handler(Registers registers){
    if(registers.int_no >= 40){ // Is interrupt using slave register?
        Ports::outb(0xA0, 0x20); // Send reset to slave.
    }

    Ports::outb(0x20, 0x20); // Send reset to master.

    ISR::handle(registers); // Handle the interrupt.
}

void IDT::init() {
    ptr.limit = sizeof(IdtEntry) * 256 -1;
    ptr.base  = (uint32) &entries;

    memset((uint8*) &entries, 0, sizeof(IdtEntry)*256);

    setGate(0, (uint32)isr0 , 0x08, 0x8E);
    setGate(1, (uint32)isr1 , 0x08, 0x8E);
    setGate(2, (uint32)isr2 , 0x08, 0x8E);
    setGate(3, (uint32)isr3 , 0x08, 0x8E);
    setGate(4, (uint32)isr4 , 0x08, 0x8E);
    setGate(5, (uint32)isr5 , 0x08, 0x8E);
    setGate(6, (uint32)isr6 , 0x08, 0x8E);
    setGate(7, (uint32)isr7 , 0x08, 0x8E);
    setGate(8, (uint32)isr8 , 0x08, 0x8E);
    setGate(9, (uint32)isr9 , 0x08, 0x8E);
    setGate(10, (uint32)isr10 , 0x08, 0x8E);
    setGate(11, (uint32)isr11 , 0x08, 0x8E);
    setGate(12, (uint32)isr12 , 0x08, 0x8E);
    setGate(13, (uint32)isr13 , 0x08, 0x8E);
    setGate(14, (uint32)isr14 , 0x08, 0x8E);
    setGate(15, (uint32)isr15 , 0x08, 0x8E);
    setGate(16, (uint32)isr16 , 0x08, 0x8E);
    setGate(17, (uint32)isr17 , 0x08, 0x8E);
    setGate(18, (uint32)isr18 , 0x08, 0x8E);
    setGate(19, (uint32)isr19 , 0x08, 0x8E);
    setGate(20, (uint32)isr20 , 0x08, 0x8E);
    setGate(21, (uint32)isr21 , 0x08, 0x8E);
    setGate(22, (uint32)isr22 , 0x08, 0x8E);
    setGate(23, (uint32)isr23 , 0x08, 0x8E);
    setGate(24, (uint32)isr24 , 0x08, 0x8E);
    setGate(25, (uint32)isr25 , 0x08, 0x8E);
    setGate(26, (uint32)isr26 , 0x08, 0x8E);
    setGate(27, (uint32)isr27 , 0x08, 0x8E);
    setGate(28, (uint32)isr28 , 0x08, 0x8E);
    setGate(29, (uint32)isr29 , 0x08, 0x8E);
    setGate(30, (uint32)isr30 , 0x08, 0x8E);
    setGate(31, (uint32)isr31 , 0x08, 0x8E);


    Ports::outb(0x20, 0x11);
    Ports::outb(0xA0, 0x11);
    Ports::outb(0x21, 0x20);
    Ports::outb(0xA1, 0x28);
    Ports::outb(0x21, 0x04);
    Ports::outb(0xA1, 0x02);
    Ports::outb(0x21, 0x01);
    Ports::outb(0xA1, 0x01);
    Ports::outb(0x21, 0x0);
    Ports::outb(0xA1, 0x0);


    setGate(32, (uint32)irq0, 0x08, 0x8E);
    setGate(33, (uint32)irq1, 0x08, 0x8E);
    setGate(34, (uint32)irq2, 0x08, 0x8E);
    setGate(35, (uint32)irq3, 0x08, 0x8E);
    setGate(36, (uint32)irq4, 0x08, 0x8E);
    setGate(37, (uint32)irq5, 0x08, 0x8E);
    setGate(38, (uint32)irq6, 0x08, 0x8E);
    setGate(39, (uint32)irq7, 0x08, 0x8E);
    setGate(40, (uint32)irq8, 0x08, 0x8E);
    setGate(41, (uint32)irq9, 0x08, 0x8E);
    setGate(42, (uint32)irq10, 0x08, 0x8E);
    setGate(43, (uint32)irq11, 0x08, 0x8E);
    setGate(44, (uint32)irq12, 0x08, 0x8E);
    setGate(45, (uint32)irq13, 0x08, 0x8E);
    setGate(46, (uint32)irq14, 0x08, 0x8E);


    idt_flush((uint32) &ptr);
}

void IDT::setGate(uint8 num, uint32 base, uint16 sel, uint8 flags) {
    entries[num].base_lo = base & 0xFFFF;
    entries[num].base_hi = (base >> 16) & 0xFFFF;

    entries[num].sel     = sel;
    entries[num].always0 = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    entries[num].flags   = flags /* | 0x60 */;
}

void IDT::flush(uint32 ptr) {
    idt_flush(ptr);
}

void IDT::enable() {
    asm volatile("sti");
}

void IDT::disable() {
    asm volatile("cli");
}
