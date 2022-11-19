#include "IO/TTY.h"

extern "C" int kmain(){
    TTY tty(VGA_COLOUR_BLACK, VGA_COLOUR_WHITE);

    tty.printk("Hello world * %d\n", 77);

    for(;;){

    }

    return 0;
}
