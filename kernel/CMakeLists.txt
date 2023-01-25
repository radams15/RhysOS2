enable_language(ASM_NASM)
enable_language(ASM)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
set(CMAKE_C_FLAGS "-ffreestanding -O2 -Wall -Wextra")
set(CMAKE_EXE_LINKER_FLAGS "-g -m32 -T ${CMAKE_SOURCE_DIR}/linker.ld -ffreestanding -O2 -nostdlib")
set(CMAKE_CXX_STANDARD 14)

set(NASM_SOURCES
        src/boot/boot.nasm)

set_source_files_properties(${NASM_SOURCES} PROPERTIES LANGUAGE ASM_NASM)

set(C_SOURCES
       src/main.cpp src/x86/IO/TTY.cpp include/IO/TTY.h include/Types.h include/VGA.h src/Math.cpp include/Math.h src/String.cpp include/String.h src/x86/IO/Ports.cpp include/IO/Ports.h src/x86/Memory/GDT.cpp include/Memory/GDT.h src/x86/Interrupts/IDT.cpp include/Interrupts/IDT.h src/Memory/Memory.cpp include/Memory/Memory.h src/x86/Interrupts/ISR.cpp include/Interrupts/ISR.h src/x86/Interrupts/Clock.cpp include/Interrupts/Clock.h src/Memory/malloc.cpp include/Memory/malloc.h src/x86/Memory/Paging.cpp include/Memory/Paging.h include/Panic.h src/libc/List.cpp include/libc/List.h src/Memory/Heap.cpp include/Memory/Heap.h src/x86/IO/Graphics.cpp include/IO/Graphics.h include/IO/Glyphs.h src/IO/Keyboard.cpp include/IO/Keyboard.h src/TextEdit.cpp include/TextEdit.h src/x86/IO/Serial.cpp include/IO/Serial.h src/IO/ATA.cpp src/FS/UStar.cpp src/libc/Syscall.cpp)

add_executable(Kernel ${NASM_SOURCES} ${ASM_SOURCES} ${C_SOURCES})

target_include_directories(Kernel PRIVATE include)
target_include_directories(Kernel PRIVATE include/x86)
