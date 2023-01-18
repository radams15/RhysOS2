//
// Created by Rhys Adams on 18/01/2023.
//

#include "IO/Graphics.h"
#include "IO/Glyphs.h"
#include "IO/Ports.h"
#include "Memory/Memory.h"

#define PALETTE_MASK 0x3C6
#define PALETTE_READ 0x3C7
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA 0x3C9

uint8* Graphics::video_mem = NULL;

void Graphics::init(uint8 *video_mem) {
    Graphics::video_mem = video_mem;

    Ports::outb(PALETTE_MASK, 0xff);
    Ports::outb(PALETTE_WRITE, 0x00);

    for(uint8 i=0 ; i<255 ; i++){
        Ports::outb(PALETTE_DATA, (((i>>5) & 0x7) * (256 / 8)) / 4);
        Ports::outb(PALETTE_DATA, (((i>>2) & 0x7) * (256 / 8)) / 4);
        Ports::outb(PALETTE_DATA, (((i>>0) & 0x7) * (256 / 8)) / 4);
    }

    Ports::outb(PALETTE_DATA, 0x3F);
    Ports::outb(PALETTE_DATA, 0x3F);
    Ports::outb(PALETTE_DATA, 0x3F);
}

void Graphics::init(unsigned char *video_mem, unsigned char *buffer) {
    init(video_mem);

    set_buffer(buffer);
}

void Graphics::set_buffer(uint8* buffer) {
    memcpy(video_mem, buffer, SCREEN_HEIGHT*SCREEN_WIDTH);
}

void Graphics::draw(unsigned int x, unsigned int y, unsigned char colour) {
    video_mem[(y*SCREEN_WIDTH)+x] = colour;
}

void Graphics::panic(char *text) {
    uint32 x, y;
    for (y = 0; y < SCREEN_HEIGHT; ++y) {
        for (x = 0; x < SCREEN_WIDTH; ++x) {
            video_mem[y*SCREEN_WIDTH+x] = 0b11100000;    // RED
        }
    }
    uint32 index = 0;
    char chr = text[index];
    while (chr != 0) {
        const uint8 *glyph = font_glyphs[(uint8) chr];      // a pointer to the character in font_glyphs in ./assets/font_glyph.h
        uint32 index_x, index_y;                            // Bit index in the glyph
        for (index_y = 0; index_y < 8; ++index_y) {
            for (index_x = 0; index_x < 8; ++index_x) {
                if (glyph[index_y] & (1 << index_x)) {
                    video_mem[4*SCREEN_WIDTH+index_y*SCREEN_WIDTH+index_x+index*8+4] = 0xFF;
                }
            }
        }
        chr = text[++index];
    }
}

void Graphics::set_bg(unsigned char colour) {
    memset(video_mem, colour, SCREEN_HEIGHT*SCREEN_WIDTH);
}
