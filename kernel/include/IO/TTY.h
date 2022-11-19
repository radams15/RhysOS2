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

    uint16 col;
    uint16 row;
    uint16* buf;

    uint16 getBufIndex(uint16 x, uint16 y);

    void setChar(int x, int y, char c);

    void print_args(const char* text, va_list args);

public:
    TTY(vga_color fg, vga_color bg);
    void setColour(vga_color fg, vga_color bg);

    void clear();
    void putc(char c);
    void puti(uint16 num);
    void printk(const char* text, ...);
};


#endif //RHYSOS_TTY_H
