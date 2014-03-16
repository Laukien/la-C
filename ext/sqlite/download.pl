#!/usr/bin/env perl

use strict;
use warnings;
use LWP::Simple;

exit 0 if (-e "sqlite3.c" && -e "sqlite3.h");

my $page = get("http://sqlite.org/download.html") || die("ERROR: couldn't load page\n");
my $start = index($page, "'a1','");
die("ERROR: id not found\n") if ($start == -1);
$start += 6;
my $stop = index($page, "');", $start);
die("ERROR: invalid format\n") if ($stop == -1);
my $url = "http://sqlite.org/" . substr($page, $start, $stop - $start);
print("download " . $url . "\n");
my $file = "sqlite.zip";
getstore($url, $file);
