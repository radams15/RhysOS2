//
// Created by rhys on 19/11/22.
//

#ifndef RHYSOS_TTY_H
#define RHYSOS_TTY_H

#include "Types.h"
#include "VGA.h"

#include <stdarg.h>

class TTY {
private:
    static uint8 colour;

    static uint8 col;
    static uint8 row;
    static uint16* buf;

    static uint16 getBufIndex(uint8 x, uint8 y);

public:
    static void init(vga_color fg, vga_color bg);
    static void setColour(vga_color fg, vga_color bg);

    static void print_args(const char* text, va_list args, bool doFormat=true);

    static void setChar(uint16 x, uint16 y, uint8 c);
    static void setChar(uint16 pos, uint8 c);

    static void clear();
    static void putc(uint8 c);
    static void puti(uint16 num);
    static void printk(const char* text, ...);

    static void putCursor(uint16 x, uint16 y);
    static void putCursor(uint16 loc);
    static void scroll();

    static uint16 currentIndex();

    static uint16* getBuf();
    static void setBuf(uint16* buf);
};


#endif //RHYSOS_TTY_H
