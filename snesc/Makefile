PREFIX=/usr/local/snes-sdk

BINDIR=$(PREFIX)/bin
AS=$(BINDIR)/wla-65816
LD=$(BINDIR)/wlalink
CC=$(BINDIR)/816-tcc
EMU=$(BINDIR)/snes9x

LIBDIR=$(PREFIX)/lib
LIBS = $(LIBDIR)/crt0_snes.obj $(LIBDIR)/libm.obj $(LIBDIR)/libtcc.obj $(LIBDIR)/libc.obj

ASMOBJ = data.obj graph.obj str.obj init_asm.obj
COBJ = snesc.obj input.obj init.obj

all: snesc.smc
	$(EMU) snesc.smc || xset r on

snesc.smc: $(ASMOBJ) $(COBJ)
	$(LD) -vS $(LIBS) $(ASMOBJ) $(COBJ) snesc.smc

%.s: %.c
	$(CC) -I. -Wl,--oformat,binary -o $@ -c $<
%.obj: %.s
	$(AS) -io $< $@
%.obj: %.asm
	$(AS) -io $< $@

clean:
	rm -f snesc.smc snesc.sym $(ASMOBJ) $(COBJ) *.s
