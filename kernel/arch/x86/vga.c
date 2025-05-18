#include "../vga.h"
#include <type.h>

static uint16* vga_mem = (uint16*) VGA_MEM;

static inline uint8 VGA_colour(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16 VGA_chr(unsigned char uc, uint8 color) {
	return (uint16) uc | (uint16) color << 8;
}


void vga_init() {

}

void vga_putc(int row, int col, int fg, int bg, char c) {
    vga_mem[col * 80 + row] = VGA_chr(c, VGA_colour(fg, bg));
}

int vga_getcols() {
    return 80;
}

int vga_getrows() {
    return 25;
}
