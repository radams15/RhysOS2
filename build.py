from os import system, mkdir, getcwd
import subprocess
from os.path import exists, splitext, isdir
from shutil import copytree, rmtree, copyfile
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
DEFS = {
    #'VIDEO': 1
    'FLOPPY': 1
}

KERN_DIRS = ["build/kernel"+x.replace("kernel/src", "") for x in glob("kernel/src/**", recursive=True) if isdir(x)]

BUILD_DIRS = [
    "build"
] + KERN_DIRS

root = getcwd()
def_str = ' '.join(f'-D{k}={v}' for k, v in DEFS.items())

CFLAGS = '-ffreestanding -Wall' # -Wextra

if platform == "darwin":
    CC = f"i386-elf-gcc"
    CXX = f"i386-elf-g++"
    AS = f"i386-elf-as"
    LD = f"i386-elf-ld"
    GRUB = "grub-mkrescue"
    GRUBISO = "grub-file"
    OBJCOPY = 'i386-elf-objcopy'
else:
    CC = f"ccache /home/rhys/scripts/i386-gnu-gcc/bin/i386-elf-gcc"
    CXX = f"ccache /home/rhys/scripts/i386-gnu-gcc/bin/i386-elf-g++"
    AS = f"/home/rhys/scripts/i386-gnu-gcc/bin/i386-elf-as"
    LD = f"/home/rhys/scripts/i386-gnu-gcc/bin/i386-elf-ld"
    GRUB = "grub2-mkrescue"
    GRUBISO = "grub2-file"
    OBJCOPY = f"/home/rhys/scripts/i386-gnu-gcc/bin/i386-elf-objcopy"

STD = "gnu99"

BOOTLOADER_BIN_FILE = f"{root}/build/bootloader.bin"
KERNEL_BIN_FILE = f"{root}/build/kernel.bin"
IMG_FILE = f"{root}/build/RhysOS.img"

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
    run_cleanly(f"nasm bootloader/boot.nasm {def_str} {'-g' if DEBUG else ''} -I bootloader -o {BOOTLOADER_BIN_FILE}")

    return BOOTLOADER_BIN_FILE

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

        command = f"nasm -f elf32 {'-g' if DEBUG else ''} {def_str} -o {obj_file} {nasm_file} "

        run_cleanly(command, tabs=1)

    def compile_c_file(c_file):
        obj_file = (splitext(c_file)[0]+".o").replace("kernel/src", "build/kernel")
        obj_files.append(obj_file)

        command = f"{CC} -std={STD} {CFLAGS} -c {c_file} {def_str} {'-g' if DEBUG else ''} -o {obj_file} -I kernel/include/"

        run_cleanly(command, tabs=1)

    def compile_cpp_file(cpp_file):
        obj_file = (splitext(cpp_file)[0]+".o").replace("kernel/src", "build/kernel")
        obj_files.append(obj_file)

        command = f"{CXX} {CFLAGS} {def_str} -c {cpp_file} -o {obj_file} {'-g' if DEBUG else ''} -I kernel/include/ -fno-exceptions -fno-rtti"

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
    command = f"{LD} -T linker.ld -o {KERNEL_BIN_FILE} -O0 -nostdlib --oformat binary "
    
    for o in object_files:
        command += o + " "
    
    run_cleanly(command, tabs=1)
    print("\n")

    return KERNEL_BIN_FILE

def make_iso(*elems):
    print("*** Make IMG ***")

    data = b''

    for elem in elems:
        with open(elem, 'rb') as f:
            data += f.read()

    with open(IMG_FILE, 'wb') as f:
        f.write(data)

    print("\n")

    """
    print("*** Make IMG ***")

    copyfile('RhysOS.template', IMG_FILE)
    run_cleanly(f'dd conv=notrunc if={bootloader} of={IMG_FILE}')
    run_cleanly(f'hdiutil attach {IMG_FILE}')

    for elem in elems:
        run_cleanly(f'cp {elem} /Volumes/BOOTFS/')

    run_cleanly('hdiutil detach /Volumes/BOOTFS')

    print("\n")"""


def run_qemu():
    print("*** Run QEMU ***")
    run_cleanly(f"qemu-system-i386 -boot a -fda {IMG_FILE} -hda disk.tar  {'-d int,cpu_reset' if DEBUG else ''} -m {MEMORY} {'-s -S' if DEBUG else ''} -serial file:{KERNEL_LOGFILE}", tabs=1)
    print("\n")


if __name__ == "__main__":
    clean()
    make_dirs()

    bootloader = comp_bootloader()
    
    objs = comp_kernel()

    kernel = link_kernel(objs)

    print(bootloader, kernel)

    make_iso(bootloader, kernel)

    run_qemu()
