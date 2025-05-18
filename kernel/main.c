#include <arch/vga.h>

int kmain() {
    vga_init();

    for(int i=0 ; i<30 ; i++) {

        for(int c='A' ; c<0xFF ; c++) {
            vga_putc(c);
        }
    }

    while(1) {

    }

    return 0;
}
