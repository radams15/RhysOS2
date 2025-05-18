#!/usr/bin/perl

use warnings;
use strict;
use utf8;
use 5.030_000;

my $ASM = 'nasm';
my $CC  = 'cross/linux/bin/i686-elf-gcc';

sub run {
    my ($cmd) = @_;

    printf "Run: '%s'\n", $cmd;
    system($cmd);
}

sub mkdir_p {
    `mkdir -p $_[0]`;
}

sub mkdirs {
    mkdir_p 'build/boot';
    mkdir_p 'build/kernel';
    mkdir_p 'build/iso/boot';
}

sub bootloader {
    run "$ASM -felf32 boot/boot.nasm -o build/boot/boot.o";

    'build/boot/boot.o';
}

sub kernel {
    my @objs;

    push @objs, bootloader;


    my $objs = join ' ', @objs;

    run "$CC -m32 -T kernel/link.ld -ffreestanding -O0 -nostdlib -o build/kernel/kernel.bin -lgcc $objs";

    'build/kernel/kernel.bin';
}

sub mkiso {
    my ($kern_bin) = @_;

    run 'rm -rf build/iso/boot/grub';
    run 'cp -r grub/ build/iso/boot/grub';
    run "cp -r $kern_bin build/iso/boot/rhysos.bin";

    run "grub2-mkrescue -o build/rhysos.iso build/iso";
}

mkdirs;

my $kernel = kernel;
mkiso $kernel;
