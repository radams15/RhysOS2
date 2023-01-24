#include "IO/ATA.h"

#include "IO/Ports.h"
#include "Interrupts/ISR.h"
#include "IO/TTY.h"
#include "Panic.h"

#define ATA_PRIMARY_IO 0x1F0
#define ATA_SECONDARY_IO 0x170

#define ATA_PRIMARY_DCR_AS 0x3F6
#define ATA_SECONDARY_DCR_AS 0x376

#define ATA_PRIMARY_IRQ 14
#define ATA_SECONDARY_IRQ 15

#define ATA_SR_BSY     0x80
#define ATA_SR_DRDY    0x40
#define ATA_SR_DF      0x20
#define ATA_SR_DSC     0x10
#define ATA_SR_DRQ     0x08
#define ATA_SR_CORR    0x04
#define ATA_SR_IDX     0x02
#define ATA_SR_ERR     0x01

#define ATA_ER_BBK      0x80
#define ATA_ER_UNC      0x40
#define ATA_ER_MC       0x20
#define ATA_ER_IDNF     0x10
#define ATA_ER_MCR      0x08
#define ATA_ER_ABRT     0x04
#define ATA_ER_TK0NF    0x02
#define ATA_ER_AMNF     0x01

#define ATA_CMD_READ_PIO          0x20
#define ATA_CMD_READ_PIO_EXT      0x24
#define ATA_CMD_READ_DMA          0xC8
#define ATA_CMD_READ_DMA_EXT      0x25
#define ATA_CMD_WRITE_PIO         0x30
#define ATA_CMD_WRITE_PIO_EXT     0x34
#define ATA_CMD_WRITE_DMA         0xCA
#define ATA_CMD_WRITE_DMA_EXT     0x35
#define ATA_CMD_CACHE_FLUSH       0xE7
#define ATA_CMD_CACHE_FLUSH_EXT   0xEA
#define ATA_CMD_PACKET            0xA0
#define ATA_CMD_IDENTIFY_PACKET   0xA1
#define ATA_CMD_IDENTIFY          0xEC

#define      ATAPI_CMD_READ       0xA8
#define      ATAPI_CMD_EJECT      0x1B

#define ATA_IDENT_DEVICETYPE   0
#define ATA_IDENT_CYLINDERS    2
#define ATA_IDENT_HEADS        6
#define ATA_IDENT_SECTORS      12
#define ATA_IDENT_SERIAL       20
#define ATA_IDENT_MODEL        54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID   106
#define ATA_IDENT_MAX_LBA      120
#define ATA_IDENT_COMMANDSETS  164
#define ATA_IDENT_MAX_LBA_EXT  200

#define IDE_ATA        0x00
#define IDE_ATAPI      0x01

#define ATA_MASTER     0x00
#define ATA_SLAVE      0x01

#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_SECCOUNT1  0x08
#define ATA_REG_LBA3       0x09
#define ATA_REG_LBA4       0x0A
#define ATA_REG_LBA5       0x0B
#define ATA_REG_CONTROL    0x0C
#define ATA_REG_ALTSTATUS  0x0C
#define ATA_REG_DEVADDRESS 0x0D

// Channels:
#define      ATA_PRIMARY      0x00
#define      ATA_SECONDARY    0x01

// Directions:
#define      ATA_READ      0x00
#define      ATA_WRITE     0x013


uint8 ata_pm = 0; /* Primary master exists? */
uint8 ata_ps = 0; /* Primary Slave exists? */
uint8 ata_sm = 0; /* Secondary master exists? */
uint8 ata_ss = 0; /* Secondary slave exists? */

uint8 *ide_buf = 0;


void isr6_handle(Registers r){

}

void isr46_handle(Registers r){

}

void ATA::init() {
    ISR::register_handler(6, isr6_handle); // Null handler as we are polling.
    ISR::register_handler(46, isr46_handle); // Null handler as we are polling.

    probe();
}

void ATA::readSects(unsigned int lba, uint8 numSects, uint8* buffer) {
    for(uint32 sect=0 ; sect < numSects ; sect++){
        readSect(lba+sect, buffer);
        buffer += 512;
    }
}

void ATA::readSect(unsigned int lba, uint8 *buffer) {
    waitBusy();

    Ports::outb(0x1F6,0xE0 | ((lba >>24) & 0x0F));
    Ports::outb(ATA_PRIMARY_IO + ATA_REG_SECCOUNT0, 1);
    Ports::outb(ATA_PRIMARY_IO + ATA_REG_LBA0, (uint8) lba);
    Ports::outb(ATA_PRIMARY_IO + ATA_REG_LBA1, (uint8)(lba >> 8));
    Ports::outb(ATA_PRIMARY_IO + ATA_REG_LBA2, (uint8)(lba >> 16));
    Ports::outb(ATA_PRIMARY_IO + ATA_REG_COMMAND,ATA_CMD_READ_PIO); //Send the read command

    waitBusy();
    waitDrq();

    for(int i=0 ; i<20 ; i+=2) { // 256
        uint16 data = Ports::inw(ATA_PRIMARY_IO + ATA_REG_DATA);

        uint8 a, b;
        a = (uint8) data;
        b = (uint8) (data >> 8);

        buffer[i] = a;
        buffer[i+1] = b;
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

void ATA::selectDrive(uint8 bus, uint8 i){
    if(bus == ATA_PRIMARY)
        if(i == ATA_MASTER)
            Ports::outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xA0);
        else Ports::outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xB0);
    else
    if(i == ATA_MASTER)
        Ports::outb(ATA_SECONDARY_IO + ATA_REG_HDDEVSEL, 0xA0);
    else Ports::outb(ATA_SECONDARY_IO + ATA_REG_HDDEVSEL, 0xB0);
}

void ATA::identify(unsigned char bus, unsigned char drive) {
    uint16 io = 0;
    selectDrive(bus, drive);
    if(bus == ATA_PRIMARY) io = ATA_PRIMARY_IO;
    else io = ATA_SECONDARY_IO;
    /* ATA specs say these values must be zero before sending IDENTIFY */
    Ports::outb(io + ATA_REG_SECCOUNT0, 0);
    Ports::outb(io + ATA_REG_LBA0, 0);
    Ports::outb(io + ATA_REG_LBA1, 0);
    Ports::outb(io + ATA_REG_LBA2, 0);
    /* Now, send IDENTIFY */
    Ports::outb(io + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
    TTY::printk("Sent IDENTIFY\n");
    /* Now, read status port */
    uint8 status = Ports::inb(io + ATA_REG_STATUS);
    if(status)
    {
        /* Now, poll untill BSY is clear. */
        while(Ports::inb(io + ATA_REG_STATUS) & ATA_SR_BSY != 0) ;
        pm_stat_read:		status = Ports::inb(io + ATA_REG_STATUS);
        if(status & ATA_SR_ERR)
        {
            TTY::printk("%s%s has ERR set. Disabled.\n", bus==ATA_PRIMARY?"Primary":"Secondary", drive==ATA_PRIMARY?" master":" slave");
            return;
        }
        while(!(status & ATA_SR_DRQ)) goto pm_stat_read;
        TTY::printk("%s%s is online.\n", bus==ATA_PRIMARY?"Primary":"Secondary", drive==ATA_PRIMARY?" master":" slave");
        /* Now, actually read the data */

        for(int i = 0; i<256; i++)
        {
            *(uint16 *)(ide_buf + i*2) = Ports::inw(io + ATA_REG_DATA);
        }
    }
}

void ATA::probe() {

    identify(ATA_PRIMARY, ATA_MASTER);

    identify(ATA_PRIMARY, ATA_SLAVE);
}

