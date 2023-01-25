#include "Memory/Memory.h"
#include "FS/UStar.h"

#include "IO/ATA.h"
#include "IO/TTY.h"

UStarFS::UStarFS(uint32 lbaStart) : lbaStart(lbaStart) {

}

int oct2bin(unsigned char *str, int size) {
    int n = 0;
    unsigned char *c = str;
    while (size-- > 0) {
        n *= 8;
        n += *c - '0';
        c++;
    }
    return n;
}

void UStarFS::fileList(fileCallback callback) {
    uint8 ptr[512];

    uint32 lba = lbaStart;

    while (memcmp(ptr + 257, (uint8*) "ustar", 5) != 0) {
        ATA::readSect(lba, ptr);
        TTY::printk("LBA=%d\n", lba);

        int filesize = oct2bin(ptr + 0x7c, 11);

        callback(ptr);
        lba += 1;
    }
}
