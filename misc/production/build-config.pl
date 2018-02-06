#!/usr/bin/env perl

use v5.12;
use strict;
use warnings;
use integer;

use Getopt::Long;
use List::Util qw(sum);

my @config;

my $timeline;

GetOptions ("timeline=s" => \$timeline);

if ($timeline) {
	open(my $fh, '<', $timeline);

	push @config, 0x01, 0x00, unpack('C*', pack('v', 20 * 8));

	my $count = 0;
	while (my $line = <$fh>) {
		chomp($line);

		my ($ts, $temp) = split /\s+/, $line;

		$ts = int(24*60*60 * $ts/24.0);
		$temp = int($temp);

		push @config, unpack('C*', pack('V', $ts));
		push @config, unpack('C*', pack('v', $temp));
		push @config, 0x00, 0x00; # padding
		$count++;
	}

	if ($count > 20) {
		die "too many timeline points ($count)";
	}

	if ($count < 20) {
		# add terminator
		push @config, 0xFF, 0xFF, 0xFF, 0xFF;
		push @config, 0x00, 0x00;
		push @config, 0x00, 0x00;
	}

	close($fh);
}

# add terminator
push @config, 0xFF, 0xFF, 0x00, 0x00;

# pad to 2048
if (scalar(@config) > 2048 - 8) {
	die "config too large (" . scalar(@config) . " bytes)";
}

while (scalar(@config) < 2048 - 8) {
	push @config, 0;
}

my $checksum = ~sum(unpack("V*", pack("C*", @config))) & 0xFFFFFFFF;

print "conf";
print pack('V', $checksum);
print pack('C*', @config);
