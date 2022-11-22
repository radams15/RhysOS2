//
// Created by rhys on 22/11/22.
//

#include "Interrupts/ISR.h"
#include "IO/TTY.h"

IsrHandler ISR::handlers[256];

extern "C" void isr_handler(Registers regs) {
    TTY::printk("Interrupt: %d\n", regs.int_no);

    ISR::handle(regs);
}

void ISR::register_handler(unsigned char isr_num, IsrHandler handler) {
    handlers[isr_num] = handler;
}

void ISR::handle(Registers registers) {
    handlers[registers.int_no](registers);
}
