//
// Created by Rhys Adams on 19/01/2023.
//

#include "TextEdit.h"
#include "IO/Keyboard.h"
#include "IO/TTY.h"
#include "IO/Keymap.h"

int8 TextEdit::buf[] = "abcdef";
uint8 TextEdit::x = 0, TextEdit::y = 1;

void TextEdit::run() {
    Keyboard::register_handler(key_press);

    //memset_s((int8*) buf, ' ', VGA_WIDTH*VGA_HEIGHT);

    redraw();
}

void TextEdit::key_press(uint32 code) {
    TTY::putCursor(x, y);

    char chr;

    if(keymap[code] == '\b'){
        buf[TTY::currentIndex()-1] = ' ';
        x--;
    } else if(keymap[code] == '\n'){
        buf[TTY::currentIndex()] = ' ';
        for(int xv=0 ; xv<VGA_WIDTH-x ; xv++){
            buf[TTY::currentIndex()+xv] = ' '; // Replace every char in the line with blanks to allow printk to not stop at NULLs.
        }
        y++;
        x=0;
    } else if((chr=keymap[code]) != NULL){
        buf[TTY::currentIndex()] = chr;
        x++;
    }

    redraw();
}

void TextEdit::redraw() {
    TTY::clear();

    for(int xv=0 ; xv<VGA_WIDTH ; xv++){
        TTY::setChar(xv, 0, '*');
    }

    TTY::putCursor(0,1);

    TTY::print_args((const char*) buf, NULL, false);

    TTY::putCursor(x, y+1);
}
