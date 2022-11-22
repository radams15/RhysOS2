//
// Created by rhys on 22/11/22.
//

#ifndef RHYSOS_ISR_H
#define RHYSOS_ISR_H

#include "Types.h"

struct Registers {
    uint32 ds;                  // Data segment selector
    uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
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
