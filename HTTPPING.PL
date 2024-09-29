#!/usr/local/bin/perl
# ################################
# (c) httpping.pl, Alumni
# Connective ping to http-server
# ################################

die ("httpping.pl <host> <packets> [-p<port>]\n") unless (scalar(@ARGV) >= 2);
$host = $ARGV[0];
$port = 80; 	# default, but you can change it
$packets = $ARGV[1];
$noop = "GET / HTTP/1.0";

for ($i = 2; $i < scalar(@ARGV); $i++)
{
	if ($ARGV[$i] =~ /^(\x2D\x70)/)
	{
		$port = substr ($ARGV[$i], 2, length($ARGV[$i]) - 2);
	}
}

open (PING, "|tcpping.exe $host $port $packets -n -cv -crlf") || die ("Error: ping executable not found\n");
print PING "$noop\n";
close (PING);