#include <multiboot.h>
#include <type.h>
#include <arch/vga.h>
#include <fs/fs.h>
#include <tty.h>

int stdin, stdout, stderr;

extern char* mem_end;

void panic(const char* msg) {
    vga_print(msg);
    for(;;){}
}

int kmain(multiboot_info_t* mbd, uint32 mb_magic) {
    vga_init();

    vga_print("Boot starting...\n");

    if(mb_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("Invalid multiboot_magic");
    }

    fs_root = devfs_init();

    stdout = open("stdout", O_WRONLY);

    mem_phys_init(mem_end, 0x100000);

    close(stdout);

    return 0;
}
