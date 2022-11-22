#include "IO/TTY.h"
#include "Memory/GDT.h"
#include "Interrupts/IDT.h"

extern "C" int kmain(){
    GDT::init();
    IDT::init();


    TTY::init(VGA_COLOUR_BLACK, VGA_COLOUR_WHITE);

    TTY::printk("Boot Complete!");

    for(;;){

    }

    return 0;
}
