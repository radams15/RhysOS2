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
    uint8 colour;

    uint8 col;
    uint8 row;
    uint16* buf;

    uint16 getBufIndex(uint8 x, uint8 y);

    void setChar(uint16 x, uint16 y, uint8 c);
    void setChar(uint16 pos, uint8 c);

    void print_args(const char* text, va_list args);

public:
    TTY(vga_color fg, vga_color bg);
    void setColour(vga_color fg, vga_color bg);

    void clear();
    void putc(uint8 c);
    void puti(uint16 num);
    void printk(const char* text, ...);

    void putCursor(uint16 x, uint16 y);
    void scroll();
};


#endif //RHYSOS_TTY_H
