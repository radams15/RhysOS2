floppy_load:
    push dx

    mov ah, 0x02
    mov al, dh
    mov ch, 0x00        ; Cylinder 0
    mov dh, 0x00        ; Head 0
    mov cl, 0x02        ; Sector (offset) 2, base 1
    int 0x13            ; Actual reading

    jc floppy_error

    pop dx
    cmp dh, al
    jne floppy_error

    ret

floppy_error:
    mov bx, DISK_ERROR_MSG
    call bprint
    jmp $

DISK_ERROR_MSG:
    db "Floppy read error!", 0