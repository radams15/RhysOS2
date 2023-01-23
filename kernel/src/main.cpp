#include "IO/TTY.h"
#include "Memory/GDT.h"
#include "Interrupts/IDT.h"
#include "Interrupts/Clock.h"
#include "Panic.h"
#include "IO/Keyboard.h"
#include "TextEdit.h"
#include "IO/Serial.h"


bool init(){
    GDT::init();
    IDT::init();

    Serial::init();

    TTY::init(VGA_COLOUR_BLACK, VGA_COLOUR_WHITE);

    Keyboard::init();

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

    Serial::write("Boot completed!\n");
    TTY::printk("Boot Complete!\n");

    TextEdit::run();

    /*Paging::init();

    uint32 b = Memory::kmalloc(8);
    TTY::printk("B: %x\n", b);*/

    return 0;
}
