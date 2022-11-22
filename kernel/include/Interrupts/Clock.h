//
// Created by rhys on 22/11/22.
//

#ifndef RHYSOS_CLOCK_H
#define RHYSOS_CLOCK_H


#include "Types.h"

class Clock {
private:
    static uint32 tick;
    static uint32 frequency;

public:
    static void init(uint32 frequency);

    static void clockTick();

    static void sleep(uint32 secs);
};


#endif //RHYSOS_CLOCK_H
