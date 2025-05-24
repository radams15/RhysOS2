global mboot
global _start

extern code
extern bss
extern mem_end

extern _kmain

MBALIGN:  equ 1<<0
MEMINFO:  equ 1<<1
VIDMODE:  equ 4h
FLAGS:    equ MBALIGN | MEMINFO
MAGIC:    equ 1BADB002h
CHECKSUM: equ -(MAGIC+FLAGS) & 0FFFFFFFFh

mboot:
    align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

    dd mboot
    dd code
    dd bss
    dd mem_end
    dd _start

    dd 0
    dd 640
    dd 480
    dd 32

_start: 
    push eax ; push multiboot data and magic
    push ebx
    cli
    call _kmain
    jmp $
