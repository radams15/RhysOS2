//
// Created by rhys on 19/11/22.
//

#include "Math.h"

int pow(int num, int power){
    int out = num;
    for(int i=0 ; i<power ; i++){
        out *= num;
    }

    return out;
}

void hex_str(uint32 n, char* out){
    int16 length = 0;

    char buf[32];

    while(n!=0)
    {
        // temporary variable to store remainder
        int temp  = 0;

        // storing remainder in temp variable.
        temp = n % 16;

        // check if temp < 10
        if(temp < 10)
        {
            buf[length] = temp + 48;
            length++;
        }
        else
        {
            buf[length] = temp + 55;
            length++;
        }

        n = n/16;
    }

    int16 index=0;

    /*out[0] = '0';
    out[1] = 'x';
    for(int16 x=length-1 ; x>=0 ; x--){
        out[index+2] = buf[x];

        index++;
    }

    out[length+2] = NULL;*/

    for(int16 x=length-1 ; x>=0 ; x--){
        out[index] = buf[x];

        index++;
    }
    out[length] = NULL;
}

uint32 num_size(uint32 num) {
    uint32 size = 0;

    while(TRUE) {
        num = num / 10;
        size++;
        if (num == 0) {
            break;
        }
    }

    return size;
}