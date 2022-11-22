//
// Created by rhys on 19/11/22.
//

#ifndef RHYSOS_VGA_H
#define RHYSOS_VGA_H

#include "Types.h"

static const uint16 VGA_WIDTH = 80;
static const uint16 VGA_HEIGHT = 25;

#define VGA_MEM_START 0xB8000

enum vga_color {
    VGA_COLOUR_BLACK = 0,
    VGA_COLOUR_BLUE = 1,
    VGA_COLOUR_GREEN = 2,
    VGA_COLOUR_CYAN = 3,
    VGA_COLOUR_RED = 4,
    VGA_COLOUR_MAGENTA = 5,
    VGA_COLOUR_BROWN = 6,
    VGA_COLOUR_LIGHT_GREY = 7,
    VGA_COLOUR_DARK_GREY = 8,
    VGA_COLOUR_LIGHT_BLUE = 9,
    VGA_COLOUR_LIGHT_GREEN = 10,
    VGA_COLOUR_LIGHT_CYAN = 11,
    VGA_COLOUR_LIGHT_RED = 12,
    VGA_COLOUR_LIGHT_MAGENTA = 13,
    VGA_COLOUR_LIGHT_BROWN = 14,
    VGA_COLOUR_WHITE = 15,
};

#define VGA_COLOUR(fg, bg) (((fg) | (bg)) << 4)
#define VGA_CHR(uc, colour) ((uint8) (uc) | (uint8) (colour) << 8)

#endif //RHYSOS_VGA_H
