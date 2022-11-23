//
// Created by rhys on 23/11/22.
//

#ifndef RHYSOS_PAGING_H
#define RHYSOS_PAGING_H

#include "Types.h"
#include "Interrupts/ISR.h"

#define PAGE_LEN 1024

struct Page {
    uint32 present: 1;
    uint32 rw: 1;
    uint32 user: 1;
    uint32 accessed: 1;
    uint32 dirty: 1;
    uint32 unused: 7;
    uint32 frame: 20;
};

struct PageTable {
    Page pages[PAGE_LEN];
};

struct PageDirectory {
    PageTable* tables[PAGE_LEN];
    uint32 physicalTables[PAGE_LEN];
    uint32 physicalAddr;
};

class Paging {
private:
    static PageDirectory* kernelDirectory;
    static PageDirectory* currentDirectory;
public:
    static void init();
    static void switchDirectory(PageDirectory* newDir);
    static Page* getPage(uint32 addr, int create, PageDirectory* directory);

    static void fault(Registers r);
};

class PageFrame {
    friend class Paging;
private:
    static uint32* frames;
    static uint32 length;

    static constexpr uint32 indexFromBit(uint32 a);
    static constexpr uint32 offsetFromBit(uint32 a);

    static void init();

public:
    static void set(uint32 addr);
    static void clear(uint32 addr);
    static uint32 test(uint32 addr);
    static uint32 first();

    static void alloc(Page* page, bool isKernel, bool isWriteable);
    static void free(Page* page);
};

#endif //RHYSOS_PAGING_H
