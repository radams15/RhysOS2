#include <arch/vga.h>

int kmain() {
    vga_init();

    vga_print("Boot starting...\n");

    while(1) {

    }

    return 0;
}
