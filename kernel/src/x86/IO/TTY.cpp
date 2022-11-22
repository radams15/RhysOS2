//
// Created by rhys on 19/11/22.
//

#include "IO/TTY.h"

#include "Math.h"
#include "String.h"
#include "IO/Ports.h"

#define FORMAT_MARK '%'

uint8 TTY::colour;
uint8 TTY::col;
uint8 TTY::row;
uint16* TTY::buf;

void TTY::init(vga_color fg, vga_color bg) {
    buf = (uint16*) VGA_MEM_START;

    row = 0;
    col = 0;

    setColour(fg, bg);

    clear();
}

void TTY::setColour(vga_color fg, vga_color bg) {
    colour = VGA_COLOUR(fg, bg);
}

void TTY::clear() {
    col = 0;
    row = 0;

    for(uint16 i=0 ; i<(VGA_HEIGHT*VGA_WIDTH) ; i++){
        setChar(i, ' ');
    }

    col = 0;
    row = 0;
}

void TTY::setChar(uint16 x, uint16 y, uint8 c) {
    uint16 index = getBufIndex(x, y);

    setChar(index, c);
}

void TTY::setChar(uint16 pos, uint8 c) {
    buf[pos] = VGA_CHR(c, colour);
}

uint16 TTY::getBufIndex(uint8 x, uint8 y) {
    return (y*VGA_WIDTH + x);
}

void TTY::putc(uint8 c) {
    switch(c){
        case '\n':
            row++;
            col = 0;
            break;

        case '\t':
            printk("    ");
            break;

        default:
            setChar(col, row, c);
            col++;
            break;
    }
    if (row >= VGA_WIDTH-1) {
        putc('\n');
    }
}

void TTY::puti(uint16 num) {
    uint32 power;
    uint32 size = num_size(num);

    power = pow(10, size - 1);

    uint32 p, i;

    while(TRUE){
        p = num % power;

        if(power > 10) {
            i = p / (power / 10);
            putc(chr(i));
        }else {
            putc(chr(p));
            break;
        }

        power /= 10;

        if(power == 0){
            break;
        }
    }
}

void TTY::printk(const char *text, ...) {
    va_list args;

    va_start(args, NULL);

    print_args(text, args);

    va_end(args);
}

void TTY::print_args(const char *text, va_list args) {
    bool skip_next = FALSE;

    for (int i=0; text[i] != NULL; i++){
        if(skip_next){
            skip_next = FALSE;
            continue;
        }

        if(text[i] == FORMAT_MARK){
            char formatter = text[i+1];

            switch(formatter){
                case 'd': // int
                    puti(va_arg(args, uint32));
                    break;

                case 'c': // char
                    putc(va_arg(args, uint32));
                    break;

                case 's': // string
                    printk(va_arg(args, char*));
                    break;

                case 'x': // hex int
                    printk("");
                    char hex_buf[32];
                    hex_str(va_arg(args, uint32), hex_buf);
                    printk(hex_buf);
                    break;

                case FORMAT_MARK:
                    putc(FORMAT_MARK);
                    break;

                default:
                    break;
            }

            skip_next = TRUE;

        }else{
            putc(text[i]);
        }
    }
}

void TTY::putCursor(uint16 x, uint16 y) {
    col=y;
    row=x;

    uint16 loc = getBufIndex(x, y);

    Ports::outb(0x3D4, 14);
    Ports::outb(0x3D5, loc >> 8);
    Ports::outb(0x3D4, 15);
    Ports::outb(0x3D5, loc);
}

void TTY::scroll() {
    for(uint32 i=0 ; i<((VGA_HEIGHT-1)*VGA_WIDTH) ; i++){
        buf[i] = buf[i+VGA_WIDTH];
    }

    for(uint32 i=((VGA_HEIGHT-1)*VGA_WIDTH); i<(VGA_HEIGHT*VGA_WIDTH) ; i++){
        buf[i] = ' ';
    }

    col--;
}