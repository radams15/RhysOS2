//
// Created by Rhys Adams on 19/01/2023.
//

#ifndef RHYSOS_KEYBOARD_H
#define RHYSOS_KEYBOARD_H

#include "Types.h"
#include "Interrupts/ISR.h"

class Keyboard {
public:
    typedef void (*Callback)(uint32 code);

    static void init();

    static void register_handler(Callback callback);

    static void key_pressed(Registers r);

private:
    static Callback handler;
};


#endif //RHYSOS_KEYBOARD_H
