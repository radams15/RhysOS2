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
#include "FS/UStar.h"


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

    TTY::printk("Ustar record size: %d\n", sizeof(UStarRecord));

    UStarFS root(0);
    root.fileList([](uint8* ptr){
        UStarRecord* rec = (UStarRecord*) ptr;
        TTY::printk("%s\n", rec->size);
    });


    return 0;
}
