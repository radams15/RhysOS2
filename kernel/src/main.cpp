#include "IO/TTY.h"
#include "Memory/GDT.h"
#include "Interrupts/IDT.h"
#include "Interrupts/Clock.h"
#include "Panic.h"
#include "Memory/Paging.h"
#include "libc/List.h"
#include "IO/Graphics.h"
#include "IO/Keyboard.h"
#include "IO/Keymap.h"

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


void key_pressed(uint32 code){
    TTY::putc(keymap[code]);
}

extern "C" int kmain(){
    int fail = init();

    if(fail){
        OUT("Boot failed!\n");
        halt();
    }

    Keyboard::register_handler(key_pressed);

    OUT("Boot Complete!\n");

    /*Paging::init();

    uint32 b = Memory::kmalloc(8);
    TTY::printk("B: %x\n", b);*/

    for(;;){

    }

    return 0;
}
