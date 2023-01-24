#include "IO/TTY.h"
#include "Memory/GDT.h"
#include "Interrupts/IDT.h"
#include "Interrupts/Clock.h"
#include "Panic.h"
#include "IO/Keyboard.h"
#include "TextEdit.h"
#include "IO/Serial.h"
#include "Memory/Paging.h"
#include "Memory/malloc.h"
#include "IO/ATA.h"


bool init(){
    GDT::init();
    IDT::init();

    Serial::init();

    TTY::init(VGA_COLOUR_BLACK, VGA_COLOUR_WHITE);

    Keyboard::init();
    ATA::init();

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

    uint8 buf[256];
    ATA::readSect(0, buf);
    for(uint16 i=0 ; i<16 ; i++){
        TTY::printk("%x\n", buf[i]);
    }


    return 0;
}
