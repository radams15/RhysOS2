#ifndef RHYSOS_SYSCALL_H
#define RHYSOS_SYSCALL_H

#include "Interrupts/ISR.h"

namespace Syscall {
    void init();
    void handler(Registers r);
}

#endif //RHYSOS_SYSCALL_H