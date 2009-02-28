OBJS = crt0_snes.obj libm.obj libtcc.obj libc.obj

%.obj: %.asm
	../wla_dx/wla-65816 -io $< $@

all: $(OBJS)

clean:
	rm -f $(OBJS)
