.include "hdr.asm"

.section ".rodata" superfree

tiles1:
.incbin "tiles1.dat"
tiles2:
.incbin "tiles2.dat"
bg1map:
.incbin "bg1.map"
bg2map:
.incbin "bg2.map"
palette:
.incbin "palette.dat"
backpal:
.incbin "backpal.dat"

.ends
