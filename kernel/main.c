#include <arch/vga.h>
#include <fs/fs.h>
#include <tty.h>

int stdin, stdout, stderr;

int kmain() {
    vga_init();

    vga_print("Boot starting...\n");

    fs_root = devfs_init();

    stdout = open("stdout", O_WRONLY);

    printf("Hello world '%x'\n", 0x12345);

    close(stdout);

    return 0;
}
