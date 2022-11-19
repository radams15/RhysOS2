enable_language(ASM_NASM)
enable_language(ASM)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
set(CMAKE_C_FLAGS "-ffreestanding -O2 -Wall -Wextra")
set(CMAKE_EXE_LINKER_FLAGS "-g -m32 -T ${CMAKE_SOURCE_DIR}/linker.ld -ffreestanding -O2 -nostdlib")

set(NASM_SOURCES
        src/boot/boot.nasm)

set_source_files_properties(${NASM_SOURCES} PROPERTIES LANGUAGE ASM_NASM)

set(C_SOURCES
       src/main.cpp src/x86/IO/TTY.cpp include/IO/TTY.h include/Types.h include/VGA.h include/Math.cpp include/Math.h src/String.cpp include/String.h)

add_executable(Kernel ${NASM_SOURCES} ${ASM_SOURCES} ${C_SOURCES})

target_include_directories(Kernel PRIVATE include)
target_include_directories(Kernel PRIVATE include/x86)
