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
#include "Math.h"


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
    root.fileList([](UStarRecord* rec, uint32 sectorStart, uint32 numSects, uint32 fileSize){

        uint8 data[512*numSects];
        ATA::readSects(sectorStart+1, numSects, (uint8*) data);

        TTY::printk("File(%s) is %d sectors long\n", rec->fileName, numSects);

        //TTY::printk("%s\n", data);
    });


    return 0;
}
