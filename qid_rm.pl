#!/usr/bin/perl
# qid_rm.pl
# for removing left-over msg queues (shell cmd: "ipcs")
# weide chang, copy-fee

# Perl code that's more understandable like C:
#
 @lines = `ipcs -q`; # get displayed lines from shell cmd ipcs -m
 
 for $line ( @lines ) # for each 'line' in array 'lines'
 {
    print "read in: $line";  # show that line read
    chop( $line );           # get rid of linefeed at end of line
 
 # split line into tokens, delimiter space(s) or tab char
    @tokens = split /[ \t]+/, $line;
 
 # if line starts with 0x000... qid is 2nd token, skip other lines
    if ( $tokens[0] =~ /^0x/ ) # if 1st token starts with 0x
    {
       print "*** removing msg q id $tokens[1]...\n"; # 2nd token shown
       `ipcrm -q $tokens[1]`;                   # shell cmd to remove
    }
 }

# Or shows the power of Perl with its built-in/default $_
#
# for ( `ipcs -q` ) # this generates an array of strings
# {
#    print;          # $_ is printed, that's the 'each'
#    chop;           # $_ is chopped
#    split /[\s]+/; # $_ is split, @_ is the array

#    if ( $_[0] =~ /^0x/ ) # if 1st token starts with 0x
#    {
#       print "*** removing msg q id $_[1]...\n"; # 2nd token is qid
#       `ipcrm -q $_[1]`;                       # shell cmd to remove
#    }
# }
