#include "IO/TTY.h"

extern "C" int kmain(){
    TTY tty(VGA_COLOUR_BLACK, VGA_COLOUR_WHITE);

    /*for(int i=0 ; i<100 ; i++) {
        tty.printk("Hello world * %d\n", i);
        tty.scroll();
    }*/


    tty.printk("Hello world\n");

    //tty.putCursor(5, 0);


    for(;;){

    }

    return 0;
}
