#include "IO/TTY.h"
#include "Memory/GDT.h"
#include "Interrupts/IDT.h"
#include "Interrupts/Clock.h"
#include "Panic.h"
#include "Memory/Paging.h"
#include "libc/List.h"

bool init(){
    GDT::init();
    IDT::init();

    TTY::init(VGA_COLOUR_BLACK, VGA_COLOUR_WHITE);

    IDT::enable();

    Clock::init();

    return FALSE;
}

extern "C" int kmain(){
    int fail = init();

    if(fail){
        TTY::printk("Boot failed!\n");
        halt();
    }

    TTY::printk("Boot Complete!\n");

    uint32 a = Memory::kmalloc(8);
    TTY::printk("A: %x\n", a);

    /*Paging::init();

    uint32 b = Memory::kmalloc(8);
    TTY::printk("B: %x\n", b);*/

    for(;;){

    }

    return 0;
}
