//
// Created by Rhys Adams on 20/01/2023.
//

#include <stdarg.h>
#include "IO/Serial.h"

#include "Math.h"
#include "String.h"
#include "IO/Ports.h"

#define SERIAL_PORT 0x3f8

void Serial::init(){
    Ports::outb(SERIAL_PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    Ports::outb(SERIAL_PORT + 0, 0x03);    // Set divisor to 3 (low byte) 38400 baud
    Ports::outb(SERIAL_PORT + 1, 0x00);    //                  (high byte)
    Ports::outb(SERIAL_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    Ports::outb(SERIAL_PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold

    write("Serial Initialised!\n");
}

bool send_buf_empty(){
    return Ports::inb(SERIAL_PORT + 5) & 0x20;
}

void Serial::writec(uint8 c){
    while(!send_buf_empty()){
        // wait for serial buffer to empty
    }

    Ports::outb(SERIAL_PORT, c);
}

void Serial::writei(uint32 num){
    uint32 num_temp = num;
    uint32 size = 0;
    uint32 power;

    while(TRUE) {
        num_temp = num_temp / 10;
        size++;
        if (num_temp == 0) {
            break;
        }
    }

    power = pow(10, size - 1);

    uint32 p, i;

    while(TRUE){
        p = num % power;

        if(power > 10) {
            i = p / (power / 10);
            writec(chr(i));
        }else {
            writec(chr(p));
            break;
        }

        power /= 10;

        if(power == 0){
            break;
        }
    }
}

void Serial::write(const char* text, ...){

    bool skip_next = FALSE;

    va_list args;

    va_start(args, NULL);
    for (int i=0; text[i] != NULL; i++){
        if(skip_next){
            skip_next = FALSE;
            continue;
        }

        if(text[i] == '%'){
            char formatter = text[i+1];

            switch(formatter){
                case 'd':
                    writei(va_arg(args, uint32));
                    break;


                case 'x':
                    write(""); // must be expression before assignment
                    char buf[32];
                    hex_str(va_arg(args, uint32), buf);
                    write(buf);
                    break;

                default:
                    break;
            }

            skip_next = TRUE;

        }else{
            writec(text[i]);
        }
    }

    va_end(args);
}