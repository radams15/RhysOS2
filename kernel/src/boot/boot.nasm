extern kmain

global _start
_start:
	call kmain
	jmp $

global halt
halt:
	jmp halt;