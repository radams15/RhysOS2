//
// Created by rhys on 23/11/22.
//

#ifndef RHYSOS_PANIC_H
#define RHYSOS_PANIC_H

#include "IO/TTY.h"

extern "C" void halt();

//#define PANIC(err) TTY::setColour(VGA_COLOUR_RED, VGA_COLOUR_BLACK); TTY::printk("Error: " err " in at " __FILE__ ":%d\n", __LINE__); halt();
//#define PANIC(err) TTY::printk(err); halt();

static inline void PANIC(const char* err){
    TTY::setColour(VGA_COLOUR_LIGHT_RED, VGA_COLOUR_BLACK);

    TTY::printk(err);
    halt();
}

#endif //RHYSOS_PANIC_H
