#include "IO/TTY.h"
#include "Memory/GDT.h"
#include "Interrupts/IDT.h"
#include "Interrupts/Clock.h"
#include "Panic.h"
#include "IO/Keyboard.h"
#include "TextEdit.h"

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

    Keyboard::init();

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

    TextEdit::run();

    /*Paging::init();

    uint32 b = Memory::kmalloc(8);
    TTY::printk("B: %x\n", b);*/

    for(;;){

    }

    return 0;
}
