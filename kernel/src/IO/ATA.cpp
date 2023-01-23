#include "IO/ATA.h"

#include "IO/Ports.h"
#include "Interrupts/ISR.h"
#include "IO/TTY.h"
#include "Panic.h"

void isr6_handle(Registers r){

}

void isr46_handle(Registers r){

}

void ATA::init() {
    /*Ports::outb(0x1F6, 0xA0);
    Ports::outb(0x1F2, 0);
    Ports::outb(0x1F3, 0);
    Ports::outb(0x1F4, 0);
    Ports::outb(0x1F5, 0);*/

    ISR::register_handler(6, isr6_handle); // Null handler as we are polling.
    ISR::register_handler(46, isr46_handle); // Null handler as we are polling.
}

void ATA::readSects(unsigned int lba, unsigned char numSects, uint32* buffer) {
    waitBusy();

    Ports::outb(0x1F6,0xE0 | ((lba >>24) & 0xF));
    Ports::outb(0x1F2,numSects);
    Ports::outb(0x1F3, (uint8) lba);
    Ports::outb(0x1F4, (uint8)(lba >> 8));
    Ports::outb(0x1F5, (uint8)(lba >> 16));
    Ports::outb(0x1F7,0x20); //Send the read command

    for (int j=0 ; j<numSects ; j++){
        waitBusy();
        waitDrq();

        for(int i=0 ; i<ATA_CHUNK ; i+=2) {
            uint16 a = Ports::inw(0x1F0);
            uint16 b = Ports::inw(0x1F0);

            TTY::printk("%c%c%c ",a, b);

            buffer[(j*ATA_CHUNK) + i] = a;
            buffer[(j*ATA_CHUNK) + i+2] = b;
        }
    }
}

void ATA::writeSects(unsigned int lba, unsigned char numSects, uint32 *data) {
    waitBusy();

    Ports::outb(0x1F6,0xE0 | ((lba >>24) & 0xF));
    Ports::outb(0x1F2,numSects);
    Ports::outb(0x1F3, (uint8) lba);
    Ports::outb(0x1F4, (uint8)(lba >> 8));
    Ports::outb(0x1F5, (uint8)(lba >> 16));
    Ports::outb(0x1F7,0x30); //Send the write command

    for (int j=0 ; j<numSects ; j++){
        waitBusy();
        waitDrq();

        for(int i=0;i<ATA_CHUNK;i++){
            Ports::outl(0x1F0, data[i]);
        }
    }
}

void ATA::waitBusy() {
    while(Ports::inb(0x1F7)&STATUS_BSY);
}

void ATA::waitDrq() {
    while(!(Ports::inb(0x1F7)&STATUS_RDY));
}
