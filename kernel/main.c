#include <arch/vga.h>

int kmain() {

    vga_putc(2, 2, 0, 15, 'A');

    while(1) {

    }

    return 0;
}
