//
// Created by rhys on 22/11/22.
//

#include "Interrupts/ISR.h"
#include "IO/TTY.h"

IsrHandler ISR::handlers[256];

extern "C" void isr_handler(Registers regs) {
    ISR::handle(regs);
}

void ISR::register_handler(unsigned char isr_num, IsrHandler handler) {
    handlers[isr_num] = handler;
}

void ISR::handle(Registers registers) {
    if(handlers[registers.int_no] != 0) {
        handlers[registers.int_no](registers);
    }else{
        TTY::printk("Unhandled interrupt: %d\n", registers.int_no);
    }
}
