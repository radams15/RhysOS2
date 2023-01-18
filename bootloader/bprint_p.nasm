[bits 32]
VIDEO_MEMORY equ 0xb8000    ; Start of Video buffer in VGA
WHITE_ON_BLACK equ 0x0f

; Prints a null terminated string pointed to by EBX
bprint_p:
    pusha
    mov edx, VIDEO_MEMORY

    bprint_p_loop:
        mov al, [ebx]   ; Store char in AL
        mov ah, WHITE_ON_BLACK  ; attributes in AH

        cmp al, 0
        je bprint_p_done

        mov [edx], ax   ; Store char and attributes to the current character cell

        add ebx, 1  ; Move 1 char
        add edx, 2  ; Each 2 bytes represent 1 char on screen, so moving 2 bytes means moving to the next character cell

        jmp bprint_p_loop

bprint_p_done:
    popa
    ret