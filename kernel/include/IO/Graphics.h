//
// Created by Rhys Adams on 18/01/2023.
//

#ifndef RHYSOS_GRAPHICS_H
#define RHYSOS_GRAPHICS_H

#include "Types.h"



class Graphics {
private:
    static uint8* video_mem;

public:
    enum {
        SCREEN_WIDTH = 320,
        SCREEN_HEIGHT = 200,
    };

    static void init(uint8* video_mem=(uint8*) 0xA0000);
    static void init(uint8* video_mem, uint8* buffer);

    static void set_buffer(uint8* buffer);

    static void draw(uint32 x, uint32 y, uint8 colour);

    static void set_bg(uint8 colour);

    static void panic(char* text);
};


#endif //RHYSOS_GRAPHICS_H
