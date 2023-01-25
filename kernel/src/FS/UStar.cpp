#include "Math.h"
#include "Memory/Memory.h"
#include "FS/UStar.h"

#include "IO/ATA.h"
#include "IO/TTY.h"

UStarFS::UStarFS(uint32 lbaStart) : lbaStart(lbaStart) {

}

void UStarFS::fileList(fileCallback callback) {
    uint8 ptr[512];

    uint32 lba = lbaStart;

    while (memcmp(ptr + 257, (uint8*) "ustar", 5) != 0) {
        ATA::readSect(lba, ptr);

        int filesize = oct2bin(ptr + 0x7c, 11);

        callback((UStarRecord*) ptr, lba);
        lba += 1;
    }
}
