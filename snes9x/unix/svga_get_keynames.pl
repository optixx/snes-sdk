#!/usr/bin/perl -w

die "USAGE: $0 /path/to/vgakeyboard.h > output.h\n" unless #$ARGV==1;

@sc=(("NULL")x256);
$max=0;
while(<>){
    /SCANCODE_(\S+)\s+(0x[0-9a-fA-F]+|\d+)/ or next;
    $n=eval($2);
    $x=ucfirst(lc($1));
    $max=$n if $max<$n;
    if($n<0 || $n>255){
        warn "SCANCODE_$1 has an out-of-range value, ignoring\n";
        next;
    }
    warn "$n is multiply defined: \"$x\" and $sc[$n]\n" if($sc[$n] ne "NULL");
    $sc[$n]='"'.$x.'"';
}

($f=uc($ARGV))=~s/[^a-zA-Z0-9]/_/g;
printf <<HEAD, $ARGV, $f, $f, $max;
/* SVGA scancode definition file, generated from %s */
#ifndef _SCANCODE_%s
#define _SCANCODE_%s

#define SVGA_SCANCODE_MAX %d

static const char *svga_keynames[SVGA_SCANCODE_MAX+1]={
HEAD
print "    ".join(", ", @sc[0..$max]);
print "\n};\n\n#endif\n";
