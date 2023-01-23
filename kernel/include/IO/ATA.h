#ifndef RHYSOS_ATA_H
#define RHYSOS_ATA_H

#include "Types.h"

// https://wiki.osdev.org/ATA_PIO_Mode#Handling_an_IRQ

namespace ATA {
    enum{
        STATUS_BSY = 0x80,
        STATUS_RDY = 0x40,
        STATUS_DRQ = 0x08,
        STATUS_DF = 0x20,
        STATUS_ERR = 0x01,
        ATA_CHUNK = 256
    };

    void init();

    void readSects(uint32 lba, uint8 numSects, uint32* buffer);
    void writeSects(uint32 lba, uint8 numSects, uint32* data);

    void waitBusy();
    void waitDrq();
}

#endif //RHYSOS_ATA_H