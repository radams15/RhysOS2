//
// Created by rhys on 19/11/22.
//

#ifndef RHYSOS_PORTS_H
#define RHYSOS_PORTS_H

#include "Types.h"

namespace Ports {
    void outb(uint16 port, uint8 value);
    uint8 inb(uint16 port);
    uint16 inw(uint16 port);
}

#endif //RHYSOS_PORTS_H
