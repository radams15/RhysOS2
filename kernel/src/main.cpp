#include "IO/TTY.h"
#include "Memory/GDT.h"
#include "Interrupts/IDT.h"
#include "Interrupts/Clock.h"
#include "Panic.h"
#include "IO/Keyboard.h"
#include "TextEdit.h"
#include "IO/Serial.h"
#include "IO/ATA.h"
#include "FS/UStar.h"
#include "libc/Syscall.h"

bool init(){
    GDT::init();
    IDT::init();

    Serial::init();

    TTY::init(VGA_COLOUR_BLACK, VGA_COLOUR_WHITE);

    Keyboard::init();
    ATA::init();

    Syscall::init();

    IDT::enable();

    Clock::init();

    return FALSE;
}

void dumpSectors(uint32 start, uint32 numSects){
    uint8 data[512];

    for(uint32 i=start ; i<start+numSects ; i++){
        ATA::readSect(i, (uint8*) data);
        TTY::printk("%s\n", data);
    }
}

extern "C" void test();

extern "C" int kmain(){
    int fail = init();

    if(fail){
        TTY::printk("Boot failed!\n");
        halt();
    }

    Serial::write("Boot completed!\n");
    TTY::printk("Boot Complete!\n");

    test();

    /*UStarFS root(0);
    root.fileList([](UStarRecord* rec, uint32 sectorStart, uint32 numSects, uint32 fileSize){
        TTY::printk("File(%s) is %d sectors long (%d)\n", rec->fileName, numSects, rec->type);

        //dumpSectors(sectorStart+1, numSects);
    });*/

    return 0;
}
