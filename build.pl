#!/usr/bin/perl

use warnings;
use strict;
use utf8;
use 5.030_000;

use File::Basename;
use File::Find qw/ find /;

my $ASM = 'nasm';
my $CC  = 'cross/linux/bin/i686-elf-gcc';

sub run {
    my ($cmd) = @_;

    printf "Run: '%s'\n", $cmd;
    my $ret = system($cmd);

    die "Error!" if $ret;
}

sub mkdir_p {
    # printf "Make directory '%s'\n", $_[0];
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

sub find_matching {
    my ($dir, $name_re) = @_;

    my @out;

    find(sub {$_ = $File::Find::name; push @out, $_ if /$name_re/}, $dir);

    @out
}

sub kernel {
    my @objs;

    push @objs, bootloader;

    for my $f (find_matching 'kernel/', qr/.*\.nasm$/) {
        print "$f\n";
        my $obj = "build/$f.o";
        mkdir_p dirname($obj);
        run "$ASM -felf32 -o $obj $f";
        push @objs, $obj;
    }

    for my $f (find_matching 'kernel/', qr/.*\.c$/) {
        print "$f\n";
        my $obj = "build/$f.o";
        mkdir_p dirname($obj);
        run "$CC -ffreestanding -fleading-underscore -Wall -Wextra -c $f -o $obj -I kernel/";
        push @objs, $obj;
    }

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
