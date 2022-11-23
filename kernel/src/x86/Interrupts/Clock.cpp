//
// Created by rhys on 22/11/22.
//

#include "Interrupts/Clock.h"
#include "Interrupts/ISR.h"
#include "IO/Ports.h"

uint32 Clock::tick = 0;
uint32 Clock::frequency = 0;

extern "C" void timer_callback([[maybe_unused]] Registers registers){
    Clock::clockTick();
}

void Clock::init(uint32 freq) {
    ISR::register_handler(IRQ0, timer_callback);

    Clock::frequency = freq;

    uint32 divisor = 1193180 / Clock::frequency;

    // Send the command byte.
    Ports::outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8 l = (uint8)(divisor & 0xFF);
    uint8 h = (uint8)( (divisor>>8) & 0xFF );

    // Send the frequency divisor.
    Ports::outb(0x40, l);
    Ports::outb(0x40, h);
}

void Clock::clockTick() {
    tick++;
}

void Clock::sleep(uint32 secs) {
    uint32 target = frequency*secs;

    while(tick < target){
        // Wait
    }
}

uint32 Clock::getFrequency() {
    return frequency;
}
