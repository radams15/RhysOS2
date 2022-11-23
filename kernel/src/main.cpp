#include "IO/TTY.h"
#include "Memory/GDT.h"
#include "Interrupts/IDT.h"
#include "Interrupts/Clock.h"
#include "Panic.h"

extern "C" int kmain(){
    GDT::init();
    IDT::init();

    TTY::init(VGA_COLOUR_BLACK, VGA_COLOUR_WHITE);

    TTY::printk("Boot Complete!\n");

    IDT::enable();

    Clock::init(60);

    Clock::sleep(1);

    TTY::printk("Done!");

    for(;;){

    }

    return 0;
}
