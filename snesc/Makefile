PREFIX=/usr/local/snes-sdk

BINDIR=$(PREFIX)/bin
AS=$(BINDIR)/wla-65816
LD=$(BINDIR)/wlalink
CC=$(BINDIR)/816-tcc
#EMU=$(BINDIR)/snes9x
EMU=bsnes

LIBDIR=$(PREFIX)/lib

ASMOBJ = data.obj graph.obj str.obj
COBJ = snesc.obj input.obj init.obj

all: snesc.smc
	$(EMU) snesc.smc || xset r on

snesc.smc: $(ASMOBJ) $(COBJ)
	$(LD) -dvSo $(ASMOBJ) $(COBJ) snesc.smc

%.s: %.c
	$(CC) -I. -o $@ -c $<
%.obj: %.s
	$(AS) -io $< $@
%.obj: %.asm
	$(AS) -io $< $@

clean:
	rm -f snesc.smc snesc.sym $(ASMOBJ) $(COBJ) *.s
