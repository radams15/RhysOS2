//
// Created by Rhys Adams on 19/01/2023.
//

#ifndef RHYSOS_TEXTEDIT_H
#define RHYSOS_TEXTEDIT_H

#include "Types.h"
#include "VGA.h"

class TextEdit {
private:
    enum {
        FILE_MAX = (VGA_HEIGHT*VGA_WIDTH)*10 // 10 pages of text.
    };

    static uint8 x, y;

    static void key_press(uint32 code);

    static int8 buf[FILE_MAX];

    static void redraw();
public:
    static void run();
};


#endif //RHYSOS_TEXTEDIT_H
