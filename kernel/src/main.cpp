#include "IO/TTY.h"
#include "Memory/GDT.h"
#include "Interrupts/IDT.h"
#include "Interrupts/Clock.h"
#include "Panic.h"
#include "Memory/Paging.h"
#include "libc/List.h"
#include "IO/Graphics.h"

#ifdef VIDEO
#define OUT Graphics::panic
#else
#define OUT TTY::printk
#endif

bool init(){
    GDT::init();
    IDT::init();

#ifdef VIDEO
    Graphics::init();
#else
    TTY::init(VGA_COLOUR_BLACK, VGA_COLOUR_WHITE);
#endif

    IDT::enable();

    Clock::init();

    return FALSE;
}

extern "C" int kmain(){
    int fail = init();

    if(fail){
        OUT("Boot failed!\n");
        halt();
    }

    OUT("Boot Complete!\n");

    /*Paging::init();

    uint32 b = Memory::kmalloc(8);
    TTY::printk("B: %x\n", b);*/

    for(;;){

    }

    return 0;
}
