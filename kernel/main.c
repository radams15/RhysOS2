#include <arch/vga.h>
#include <fs/fs.h>

int kmain() {
    vga_init();

    vga_print("Boot starting...\n");

    fs_root = devfs_init();

    int stdout = open("stdout", O_WRONLY);

    write(stdout, "test", 5);

    while(1) {

    }

    close(stdout);

    return 0;
}
