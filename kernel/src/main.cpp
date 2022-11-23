#include "IO/TTY.h"
#include "Memory/GDT.h"
#include "Interrupts/IDT.h"
#include "Interrupts/Clock.h"
#include "Panic.h"
#include "Memory/Paging.h"
#include "libc/List.h"

bool init(){
    GDT::init();
    IDT::init();

    TTY::init(VGA_COLOUR_BLACK, VGA_COLOUR_WHITE);

    IDT::enable();

    Clock::init();

    return FALSE;
}

extern "C" int kmain(){
    int fail = init();

    if(fail){
        TTY::printk("Boot failed!\n");
        halt();
    }

    TTY::printk("Boot Complete!\n");

    List<int> a(10, (List<int>::Comparator) *[](int a, int b){
        return (int8) a>b;
    });

    a.insert(1);
    a.insert(2);

    TTY::printk("0: %d\n", a.get(0));
    TTY::printk("1: %d\n", a.get(1));
    TTY::printk("2: %d\n", a.get(2));

    Paging::init();

    for(;;){

    }

    return 0;
}
