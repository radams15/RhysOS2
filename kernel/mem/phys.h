#ifndef MEM_PHYS_H
#define MEM_PHYS_H

int mem_phys_init(char* start, int length);

void* pmalloc(int length);

void pfree(char* ptr);

#endif
