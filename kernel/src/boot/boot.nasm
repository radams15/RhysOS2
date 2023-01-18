extern kmain

global _start
_start:
	call kmain
	jmp $

extern halt

halt:
	jmp halt;