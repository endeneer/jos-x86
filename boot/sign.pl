#!/usr/bin/perl

# BB is filehandle
open(BB, $ARGV[0]) || die "open $ARGV[0]: $!";

# set filehandle to binary mode
binmode BB;
my $buf;
read(BB, $buf, 1000);
$n = length($buf);

if($n > 510){
	print STDERR "boot block too large: $n bytes (max 510)\n";
	exit 1;
}

print STDERR "boot block is $n bytes (max 510)\n";

# not 512-$n because the last two bytes are \x55\xAA
$buf .= "\0" x (510-$n);
$buf .= "\x55\xAA";

# > means open file in write mode
open(BB, ">$ARGV[0]") || die "open >$ARGV[0]: $!";
binmode BB;
print BB $buf;
close BB;
