#include <multiboot.h>
#include <type.h>
#include <arch/vga.h>
#include <fs/fs.h>
#include <tty.h>

int stdin, stdout, stderr;

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

    char* mem_start = 0x1000000;
    int mem_len = 0x3EE00000;

    /* int i; */
    /* for(i = 0; i < mbd->mmap_length;  */
    /*     i += sizeof(multiboot_memory_map_t)) { */
    /*     multiboot_memory_map_t* mmmt =  */
    /*         (multiboot_memory_map_t*) (mbd->mmap_addr + i); */
    /*  */
    /*  */
    /*  */
    /*     if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) { */
    /*         printf("Start Addr: %x,%xh | Length: %x,%xh\n", */
    /*             mmmt->addr_low, mmmt->addr_high, mmmt->len_low, mmmt->len_high); */
    /*  */
    /*     } */
    /* } */
    

    mem_phys_init(mem_start, mem_len);

    close(stdout);

    return 0;
}
