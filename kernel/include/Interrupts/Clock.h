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
    static void init(uint32 freq=60);

    static void clockTick();

    static void sleep(uint32 secs);

    static uint32 getFrequency();
};


#endif //RHYSOS_CLOCK_H
