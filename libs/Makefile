OBJS = crt0_snes.obj libm.obj libtcc.obj libc.obj

%.pasm: %.c
	../tcc-65816/816-tcc $(CFLAGS) -I. -o $@ -c $<
	sed -i "" 's/.include "hdr.asm"//' $@

%.asm: %.pasm
	python ../tcc-65816/816-opt.py $< >$@

%.obj: %.asm
	../wla_dx/wla-65816 -io $< $@


all: $(OBJS)

libc.obj: libc.asm malloc.asm

clean:
	rm -f $(OBJS) malloc.asm
