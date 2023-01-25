#include "Math.h"
#include "Memory/Memory.h"
#include "FS/UStar.h"

#include "IO/ATA.h"
#include "IO/TTY.h"

UStarFS::UStarFS(uint32 sectorStart) : sectorStart(sectorStart) {

}

void UStarFS::fileList(fileCallback callback) {
    uint8 ptr[512];

    uint32 currentSector = sectorStart;

    ATA::readSect(currentSector, ptr);
    while (memcmp(ptr + 257, (uint8*) "ustar", 5) == 0) {
        UStarRecord* rec = (UStarRecord*) ptr;

        int fileSize = oct2bin((uint8*) rec->size, 11);
        int numSects = (fileSize/512)+1;

        callback(rec, currentSector, numSects, fileSize);

        currentSector += numSects + 1; // Go past the file content and onto the next sector.

        ATA::readSect(currentSector, ptr);
    }
}
