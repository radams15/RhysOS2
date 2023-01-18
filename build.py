from os import system, mkdir, getcwd
import subprocess
from os.path import exists, splitext, isdir
from shutil import copytree, rmtree
import datetime
from glob import glob
from sys import platform

from multiprocessing.pool import ThreadPool as Pool

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

DEBUG = False # Start with GDB

KERN_DIRS = ["build/kernel"+x.replace("kernel/src", "") for x in glob("kernel/src/**", recursive=True) if isdir(x)]

BUILD_DIRS = [
    "build",

    "build/iso",
    "build/iso/boot",
    "build/iso/boot/grub"
] + KERN_DIRS

root = getcwd()

if platform == "darwin":
    CC = f"i386-elf-gcc"
    CXX = f"i386-elf-g++"
    AS = f"i386-elf-as"
    LD = f"i386-elf-ld"
    GRUB = "grub-mkrescue"
    GRUBISO = "grub-file"
    OBJCOPY = 'i386-elf-objcopy'
else:
    CC = f"ccache {root}/cross/{platform}/bin/i686-elf-gcc"
    CXX = f"ccache {root}/cross/{platform}/bin/i686-elf-g++"
    AS = f"{root}/cross/{platform}/bin/i686-elf-as"
    LD = f"{root}/cross/{platform}/bin/i686-elf-ld"
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

    print(("\t"*tabs) + f"{Colour.OKBLUE}[{datetime.datetime.now().strftime('%S:%M:%H')}]{Colour.WARNING}\t{args}{Colour.ENDC}\t=>\t[{Colour.OKGREEN+'SUCCESS' if res.returncode == 0 else Colour.FAIL+'FAIL'}]{Colour.ENDC}", flush=True)


    if res.returncode not in [0]:
        exit(1)


def clean():
    for d in reversed(list(filter(lambda x: exists(x), BUILD_DIRS))):
        rmtree(d, ignore_errors=True)


def make_dirs():
    for d in filter(lambda x: not exists(x), BUILD_DIRS):
        mkdir(d)

def comp_bootloader():
    run_cleanly(f"nasm bootloader/boot.nasm {'-g' if DEBUG else ''} -I bootloader -o build/boot.bin")

    return 'build/boot.bin'

def comp_kernel():
    obj_files = []
    
    asm_files = [f for f in glob("kernel/src/**/*.S", recursive=True) if f != "kernel/src/boot/boot.nasm"]
    nasm_files = list(glob("kernel/src/**/*.nasm", recursive=True))

    c_files = glob("kernel/src/**/*.c", recursive=True)
    cpp_files = glob("kernel/src/**/*.cpp", recursive=True)

    files = list(set(asm_files + c_files))

    def compile_nasm_file(nasm_file):
        obj_file = (splitext(nasm_file)[0]+".o").replace("kernel/src", "build/kernel")
        obj_files.append(obj_file)

        command = f"nasm -f elf32 {'-g' if DEBUG else ''} -o {obj_file} {nasm_file} "

        run_cleanly(command, tabs=1)

    def compile_c_file(c_file):
        obj_file = (splitext(c_file)[0]+".o").replace("kernel/src", "build/kernel")
        obj_files.append(obj_file)

        command = f"{CC} -std={STD} -ffreestanding -Wall -Wextra -c {c_file} {'-g' if DEBUG else ''} -o {obj_file} -I kernel/include/"

        run_cleanly(command, tabs=1)

    def compile_cpp_file(cpp_file):
        obj_file = (splitext(cpp_file)[0]+".o").replace("kernel/src", "build/kernel")
        obj_files.append(obj_file)

        command = f"{CXX} -ffreestanding -Wall -Wextra  -c {cpp_file} -o {obj_file} {'-g' if DEBUG else ''} -I kernel/include/ -fno-exceptions -fno-rtti"

        run_cleanly(command, tabs=1)

    pool = Pool(20)

    print("\n*** Kernel Compile - NASM ***")

    pool.map(compile_nasm_file, nasm_files)

    print("\n*** Kernel Compile - C ***")

    pool.map(compile_c_file, files)

    print("\n*** Kernel Compile - C++ ***")

    pool.map(compile_cpp_file, cpp_files)


    print("\n")

    return obj_files

def link_kernel(object_files):
    print("*** Link Kernel ***")
    command = f"{LD} -T linker.ld -o {BIN_FILE} --oformat binary "
    
    for o in object_files:
        command += o + " "
    
    run_cleanly(command, tabs=1)
    print("\n")

    return BIN_FILE

def make_iso(*elems):
    print("*** Make IMG ***")

    data = b''

    for elem in elems:
        with open(elem, 'rb') as f:
            data += f.read()

    with open('build/os.img', 'wb') as f:
        f.write(data)

    print("\n")


def run_qemu():
    print("*** Run QEMU ***")
    run_cleanly(f"qemu-system-i386 -fda build/os.img -m {MEMORY} {'-s -S' if DEBUG else ''} -serial file:{KERNEL_LOGFILE}", tabs=1) # -drive format=raw,file=filesystem.cpio -serial file:{KERNEL_LOGFILE}
    print("\n")


if __name__ == "__main__":
    clean()
    make_dirs()

    bootloader = comp_bootloader()
    
    objs = comp_kernel()
    kernel = link_kernel(objs)

    make_iso(bootloader, kernel)

    run_qemu()
