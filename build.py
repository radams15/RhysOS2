from os import system, mkdir, getcwd
import subprocess
from os.path import exists, splitext, isdir
from shutil import copytree, rmtree
import datetime
from glob import glob
from sys import platform


class Colour:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

KERN_DIRS = ["build/kernel"+x.replace("kernel/src", "") for x in glob("kernel/src/**", recursive=True) if isdir(x)]

BUILD_DIRS = [
    "build",

    "build/iso",
    "build/iso/boot",
    "build/iso/boot/grub"
] + KERN_DIRS

root = getcwd()

if platform == "darwin":
    CC = f"i686-elf-gcc"
    CXX = f"i686-elf-g++"
    AS = f"i686-elf-as"
    GRUB = "grub-mkrescue"
    GRUBISO = "grub-file"
    OBJCOPY = 'i686-elf-objcopy'
else:
    CC = f"{root}/cross/{platform}/bin/i686-elf-gcc"
    CXX = f"{root}/cross/{platform}/bin/i686-elf-g++"
    AS = f"{root}/cross/{platform}/bin/i686-elf-as"
    GRUB = "grub2-mkrescue"
    GRUBISO = "grub2-file"
    OBJCOPY = f"{root}/cross/{platform}/bin/i686-elf-objcopy"

STD = "gnu99"

BIN_FILE = f"{root}/build/iso/boot/RhysOS.bin"
ISO_FILE = f"{root}/build/RhysOS.iso"

KERNEL_LOGFILE = f"{root}/kernel_log.txt"
MEMORY = 128


def run_cleanly(args, tabs=0, cwd=root):
    res = subprocess.run(args, shell=True, cwd=cwd, stdout=subprocess.PIPE)#, stderr=subprocess.PIPE)

    if res.returncode not in [0]:
        #print(("\t"*tabs) + f"{Colour.FAIL}{res.stderr.decode()}{Colour.ENDC}")
        exit(1)

    print(("\t"*tabs) + f"{Colour.OKBLUE}[{datetime.datetime.now().strftime('%S:%M:%H')}]{Colour.WARNING}\t{args}{Colour.ENDC}\t=>\t[{Colour.OKGREEN+'SUCCESS' if res.returncode == 0 else Colour.FAIL+'FAIL'}]{Colour.ENDC}", flush=True)


def clean():
    for d in reversed(list(filter(lambda x: exists(x), BUILD_DIRS))):
        rmtree(d, ignore_errors=True)


def make_dirs():
    for d in filter(lambda x: not exists(x), BUILD_DIRS):
        mkdir(d)

def comp_fonts():
    font = 'res/fonts/zap-vga09.psf'
    out = 'build/font.o'

    run_cleanly(f"{OBJCOPY} -O elf32-i386 -B i386 -I binary {font} {out}")

    return [out]

def comp_kernel():
    obj_files = []

    #system(f"nasm -f elf32 kernel/src/boot/boot.nasm -o build/kernel/boot/boot.o")
    #obj_files.append("build/kernel/boot/boot.o")
    
    asm_files = [f for f in glob("kernel/src/**/*.S", recursive=True) if f != "kernel/src/boot/boot.nasm"]
    
    c_files = glob("kernel/src/**/*.c", recursive=True)
    cpp_files = glob("kernel/src/**/*.cpp", recursive=True)

    files = list(set(asm_files + c_files))

    print("*** Kernel Compile - NASM ***")

    for nasm_file in glob("kernel/src/**/*.nasm", recursive=True):
        obj_file = (splitext(nasm_file)[0]+".o").replace("kernel/src", "build/kernel")
        obj_files.append(obj_file)

        command = f"nasm -f elf32 -o {obj_file} {nasm_file} "

        run_cleanly(command, tabs=1)
    print("\n")


    print("*** Kernel Compile - C ***")

    for c_file in files:
        obj_file = (splitext(c_file)[0]+".o").replace("kernel/src", "build/kernel")
        obj_files.append(obj_file)
        
        command = f"{CC} -std={STD} -ffreestanding -Wall -Wextra -c {c_file} -o {obj_file} -I kernel/include/"
        
        run_cleanly(command, tabs=1)

    print("*** Kernel Compile - C++ ***")

    for cpp_file in cpp_files:
        obj_file = (splitext(cpp_file)[0]+".o").replace("kernel/src", "build/kernel")
        obj_files.append(obj_file)

        command = f"{CXX} -ffreestanding -Wall -Wextra  -c {cpp_file} -o {obj_file} -I kernel/include/ -fno-exceptions -fno-rtti"

        run_cleanly(command, tabs=1)


    print("\n")

    return obj_files

def link_kernel(object_files):
    print("*** Link Kernel ***")
    command = f"{CXX} -T linker.ld  -ffreestanding -O0 -nostdlib -o {BIN_FILE} -lgcc "
    
    for o in object_files:
        command += o + " "
    
    run_cleanly(command, tabs=1)
    print("\n")

def make_iso():
    print("*** Make ISO ***")
    rmtree("build/iso/boot/grub")
    copytree(f"grub/", "build/iso/boot/grub", False, None)
    
    run_cleanly(f"{GRUB} -o {ISO_FILE} build/iso", tabs=1)
    print("\n")


def run_qemu():
    print("*** Run QEMU ***")
    run_cleanly(f"qemu-system-x86_64 -cdrom {ISO_FILE} -m {MEMORY} -serial file:{KERNEL_LOGFILE}", tabs=1) # -drive format=raw,file=filesystem.cpio -serial file:{KERNEL_LOGFILE}
    print("\n")


def check_iso():
    print("*** Check Multiboot ***")
    run_cleanly(f"{GRUBISO} --is-x86-multiboot {BIN_FILE}", tabs=1)
    print("\n")


if __name__ == "__main__":
    clean()
    make_dirs()
    
    objs = comp_kernel()
    objs += comp_fonts()
    link_kernel(objs)

    make_iso()
    check_iso()

    run_qemu()
