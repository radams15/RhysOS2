//
// Created by rhys on 23/11/22.
//

#ifndef RHYSOS_PANIC_H
#define RHYSOS_PANIC_H

#include "IO/TTY.h"

extern "C" void halt();

#define PANIC(err) TTY::setColour(VGA_COLOUR_RED, VGA_COLOUR_BLACK); TTY::printk("Error: " err " in at " __FILE__ ":%d\n", __LINE__); halt();

#endif //RHYSOS_PANIC_H
