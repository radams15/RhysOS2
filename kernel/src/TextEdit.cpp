//
// Created by Rhys Adams on 19/01/2023.
//

#include "TextEdit.h"
#include "IO/Keyboard.h"
#include "IO/TTY.h"
#include "IO/Keymap.h"

char TextEdit::buf[] = "";
uint8 TextEdit::x = 0, TextEdit::y = 1;

void TextEdit::run() {
    Keyboard::register_handler(key_press);

    redraw();
}

void TextEdit::key_press(uint32 code) {
    char chr;
    if((chr=keymap[code]) != NULL){
        buf[TTY::currentIndex()] = chr;
        TTY::putCursor(x, y);
        x++;
    }
    redraw();
}

void TextEdit::redraw() {
    TTY::clear();

    for(int xv=0 ; xv<VGA_WIDTH ; xv++){
        //TTY::setChar(xv, 0, '*');
    }

    //TTY::putCursor(0,1);

    TTY::printk(buf);
}
