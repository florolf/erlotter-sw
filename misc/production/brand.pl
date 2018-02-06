#!/usr/bin/env perl

use v5.12;
use strict;
use warnings;

use Fcntl ':seek';

if (@ARGV < 2) {
	say STDERR "usage: $0 file id";
	exit 1;
}

my $id = $ARGV[1];
if ($id =~ /^0x/) {
	$id = hex($id);
}

if ($id !~ /^[0-9]+$/) {
	die "id must be a number";
}

if ($id >= 0xFFF) {
	die "id is out of range";
}

open(my $fh, '+<:raw', $ARGV[0]) or die "opening file failed: $!";

seek $fh, 8, SEEK_SET;
print $fh pack('V', $id);

close($fh);
