#include "libc/Syscall.h"
#include "IO/TTY.h"

int testsc(){
    TTY::printk("Test syscall!\n");

    return 69;
}

static void* const syscalls[] = {
        (void*) &TTY::printk,
        (void*) &testsc
};
static const uint32 numSyscalls = 2;

void Syscall::init() {
    ISR::register_handler(30, (IsrHandler) handler);
}

typedef void (*func_type)(...);

void Syscall::handler(Registers regs) {
    TTY::printk("Syscall %d!\n", regs.eax);
    if (regs.eax >= numSyscalls) {
        return;
    }

    void* location = syscalls[regs.eax];

    TTY::printk("EBX: %s\n", regs.ebx);

    // We don't know how many parameters the function wants, so we just
    // push them all onto the stack in the correct order. The function will
    // use all the parameters it wants, and we can pop them all back off afterwards.
    int ret;
    asm volatile (" \
     push %1; \
     push %2; \
     push %3; \
     push %4; \
     push %5; \
     call *%6; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
   " : "=a" (ret) : "r" (regs.edi), "r" (regs.esi), "r" (regs.edx), "r" (regs.ecx), "r" (regs.ebx), "r" (location));
    regs.eax = ret; // TODO as regs are passed by value this statement has no effect.
}
