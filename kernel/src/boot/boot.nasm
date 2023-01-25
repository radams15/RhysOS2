extern kmain

global _start
_start:
	call kmain
	jmp $

global test
test:
    mov eax, 1
    int 30

    mov ecx, eax
    mov ebx, MSG_TEST
    mov eax, 0
    int 30

    ret

global halt
halt:
	jmp halt;

.data

MSG_TEST    db  "TEST %d!", 0