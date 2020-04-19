#!/usr/bin/perl

system 'rm -rf ./bin ./obj';
system 'mkdir ./bin ./obj';

opendir my $dir, './';
my @files = readdir $dir;
closedir $dir;

my @outputs = ();

foreach $file (@files) {
    my ($name, $extension) = split '\.', $file;

    if ($extension eq 'cpp') {
        my $output = "obj/$name.o";

        push @outputs, $output;
        system "g++ -Wall -g -c $file -o $output";
    }
}

system 'g++ '.(join ' ', @outputs).' -o bin/covidboy /usr/lib/x86_64-linux-gnu/libSDL2.so';
system 'xterm -e ./bin/covidboy';

1;
