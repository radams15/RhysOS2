#include "phys.h"


static char* mem_start;
static char* mem_end;
static char* mem_ptr;

int mem_phys_init(char* start, int length) {
    mem_start = start;
    mem_end = start + length;
    mem_ptr = mem_start;
    return 0;
}

void* pmalloc(int length) {
   char* out = mem_ptr;
   mem_ptr += length;
   return out;
}

void pfree(char* ptr) {

}
