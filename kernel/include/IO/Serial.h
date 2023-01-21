//
// Created by Rhys Adams on 20/01/2023.
//

#ifndef RHYSOS_SERIAL_H
#define RHYSOS_SERIAL_H

#include "Types.h"

namespace Serial {
    void init();

    void writec(uint8 c);
    void writei(uint32 i);
    void write(const char* text, ...);
}


#endif //RHYSOS_SERIAL_H
