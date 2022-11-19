MBALIGN equ  1<<0             ; align loaded modules on page boundaries
MEMINFO equ  1<<1             ; provide memory map
VIDMODE equ  0x4
FLAGS equ MBALIGN | MEMINFO ; | VIDMODE  ; this is the Multiboot 'flag' field
MAGIC equ 0x1BADB002       ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS) ; checksum of above, to prove we are multiboot

section .multiboot
    align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack_bottom:
resb 16384
stack_top:

section .text

global _start

extern kmain

_start:
	push ebx
	cli
	call kmain
