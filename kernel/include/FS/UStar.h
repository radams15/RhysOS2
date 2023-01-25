#ifndef RHYSOS_USTAR_H
#define RHYSOS_USTAR_H

#include "Types.h"

struct UStarRecord {
    char fileName[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char modified[12];
    char checksum[8];
    uint8 type;
    char linkName[100];
    char ustar[6];
    char ver[2];
    char username[32];
    char groupname[32];
    char devMaj[8];
    char devMin[8];
    char filePrefix[155];
} __attribute__((__packed__));

typedef void (*fileCallback)(UStarRecord*, uint32);


class UStarFS {
private:
    uint32 lbaStart;

public:
    explicit UStarFS(uint32 lbaStart);

    void fileList(fileCallback callback);
};

#endif //RHYSOS_USTAR_H