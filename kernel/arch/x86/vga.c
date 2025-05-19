#include "../vga.h"
#include <type.h>

static uint16* mem;
static uint32 row, col;
static enum vga_colour bg = VGA_COLOUR_BLACK, fg = VGA_COLOUR_WHITE;

static inline uint8 VGA_colour(enum vga_colour fg, enum vga_colour bg) {
	return fg | bg << 4;
}

static inline uint16 VGA_chr(unsigned char uc, uint8 color) {
	return (uint16) uc | (uint16) color << 8;
}


void vga_init() {
    row = 0;
    col = 0;
    mem = (uint16*) VGA_MEM;

    vga_clear();
}

void vga_clear() {
    for(int x=0 ; x<VGA_ROW ; x++) {
        for(int y=0 ; y<VGA_COL ; y++) {
            vga_setc(' ', x, y);
        }
    }
}

void vga_setc(char c, int row, int col) {
    mem[row * VGA_COL + col] = VGA_chr(c, VGA_colour(fg, bg));
}

void vga_scroll(int n) {
    for(int r=n ; r<VGA_ROW ; r++) {
        for(int c=0 ; c<VGA_COL ; c++) {
            mem[(r-1) * VGA_COL + c] = mem[r * VGA_COL + c]; // Copy to line above
        }
    }

    for(int i=0 ; i<n ; i++) {
        for(int c=0 ; c<VGA_COL ; c++) {
            vga_setc(' ', VGA_ROW-(n-1), col);
        }
    }

    row = 0;
}

void vga_putc(char c) {
    switch(c) {
        case '\n':
            row++;
            col = 0;
            break;
        
        case '\t':
            col += 4;
            if (col >= VGA_COL)
                row = VGA_COL-1;
            break;

        default:
            vga_setc(c, row, col);
            col++;
            break;
    }

    if(col == VGA_COL) {
        row++;
        col = 0;
    }

    if(row == VGA_ROW) {
        vga_scroll(1);
        col = 0;
    }
}

void vga_print(char* str) {
    for(char* c=str ; *c != 0 ; c++) {
        vga_putc(*c);
    }
}
