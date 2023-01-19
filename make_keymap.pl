use warnings;
use strict;

my $OUT_FILE = 'kernel/include/IO/Keymap.h';

my %keymap = (
	0x10 => 'q',
	0x14 => 't',
	0x18 => 'o',
	0x20 => 'd',
	0x24 => 'j',
	0x2C => 'z',
	0x30 => 'b',
	0x11 => 'w',
	0x15 => 'y',
	0x19 => 'p',
	0x21 => 'f',
	0x25 => 'k',
	0x2D => 'x',
	0x31 => 'n',
	0x12 => 'e',
	0x16 => 'u',
	0x1E => 'a',
	0x22 => 'g',
	0x26 => 'l',
	0x2E => 'c',
	0x32 => 'm',
	0x13 => 'r',
	0x17 => 'i',
	0x1F => 's',
	0x23 => 'h',
	0x2F => 'v',
	0x1C => '\n',
	0x0F => '\t',
	0x0E => '\b',
	0x39 => ' '
);

my ($largest_code) = sort {$b <=> $a} keys %keymap;

open FH, '>', $OUT_FILE;

print FH "#ifndef RHYSOS_KEYMAP_H\n#define RHYSOS_KEYMAP_H\n\n#include \"Types.h\"\n\nint32 keymap[] = {";
for my $i (0..$largest_code) {
    my $char = (exists $keymap{$i})? $keymap{$i} : ' ';

    print FH "\t'$char',\n";
}
print FH "\tNULL\n};\n\n#endif\n";

close FH;