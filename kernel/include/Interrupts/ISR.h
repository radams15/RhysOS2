//
// Created by rhys on 22/11/22.
//

#ifndef RHYSOS_ISR_H
#define RHYSOS_ISR_H

#include "Types.h"

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

struct Registers {
    uint32 ds;                  // Data segment selector
    uint32 edi, esi, ebp, invalid, ebx, edx, ecx, eax; // Pushed by pusha.
    uint32 int_no, err_code;    // Interrupt number and error code (if applicable)
    uint32 eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
};

typedef void (*IsrHandler)(Registers);

class ISR {
private:
    static IsrHandler handlers[256];
public:
    static void register_handler(uint8 isr_num, IsrHandler handler);

    static void handle(Registers registers);
};


#endif //RHYSOS_ISR_H
