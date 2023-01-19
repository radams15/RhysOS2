//
// Created by Rhys Adams on 19/01/2023.
//

#include "IO/Keyboard.h"

#include "IO/Ports.h"

Keyboard::Callback Keyboard::handler = NULL;

void Keyboard::init() {
    ISR::register_handler(IRQ1, Keyboard::key_pressed);
}

void Keyboard::register_handler(Keyboard::Callback callback) {
    handler = callback;
}

void Keyboard::key_pressed(Registers r) {
    uint32 code = Ports::inb(0x60);

    if(handler != NULL){
        handler(code);
    }

    Ports::outb(0x20, 0x20);
}
