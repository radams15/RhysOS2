#!/usr/bin/perl

use warnings;
use strict;

my $FOLDER = 'root';
my $OUT = 'disk.tar';

#my $SIZE = 10; # Size in sectors

#my $BYTE_PER_SECT = 512;

#open FH, '>disk.img';

#for my $i (0..($SIZE*$BYTE_PER_SECT)) {
#	print FH pack('S', $i);
#}

#close FH;

`tar cf $OUT $FOLDER/*`;