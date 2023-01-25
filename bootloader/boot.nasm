[org 0x7c00]

jmp boot

TIMES 3-($-$$) DB 0x90   ; Support 2 or 3 byte encoded JMPs before BPB.

OEMname:           db    "RHYSOS  "  ; mkfs.fat is what OEMname mkdosfs uses
bytesPerSector:    dw    512
sectPerCluster:    db    1
reservedSectors:   dw    1
numFAT:            db    2
numRootDirEntries: dw    224
numSectors:        dw    2880
mediaType:         db    0xf0
numFATsectors:     dw    9
sectorsPerTrack:   dw    18
numHeads:          dw    2
numHiddenSectors:  dd    0
numSectorsHuge:    dd    0
driveNum:          db    0
reserved:          db    0
signature:         db    41
volumeID:          dd    00000000h
volumeLabel:       db    "BOOTFS     "
fileSysType:       db    "FAT12   "

boot:
    KERNEL_OFFSET equ 0x9000
    mov [BOOT_DRIVE], dl
    mov bp, (KERNEL_OFFSET-0x2)
    mov sp,bp

    mov bx, MSG_REAL
    call bprint

    call load_kernel

    call set_protected_mode

    jmp $

%include "bprint.nasm"
%include "disk/floppy.nasm"
%include "disk/hdd.nasm"
%include "gdt.nasm"
%include "protected_mode.nasm"

[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET

    mov dh, 40

    mov dl, [BOOT_DRIVE]
%ifdef FLOPPY
    call floppy_load
%else
    call hdd_load
%endif
    ret

[bits 32]
start_kernel:
    mov ebx, MSG_PROTECTED
    call bprint_p

    call KERNEL_OFFSET

    jmp $

BOOT_DRIVE      db 0
MSG_REAL        db "Entering real mode", 0
MSG_LOAD        db "Reading kernel", 0
MSG_PROTECTED   db "Entering protected mode", 0
MSG_KERNEL      db "Loading kernel", 0

times 510-($-$$) db 0
dw 0xaa55