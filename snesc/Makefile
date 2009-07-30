PREFIX=/usr/local/snes-sdk
OPTIMIZE=1

BINDIR=$(PREFIX)/bin
AS=$(BINDIR)/wla-65816
LD=$(BINDIR)/wlalink
CC=$(BINDIR)/816-tcc
COPT=$(BINDIR)/816-opt
#EMU=$(BINDIR)/snes9x
EMU=bsnes

LIBDIR=$(PREFIX)/lib

ASMOBJ = data.obj str_asm.obj
COBJ = snesc.obj input.obj init.obj graph.obj str.obj

all: snesc.smc
	$(EMU) snesc.smc || xset r on

snesc.smc: $(ASMOBJ) $(COBJ)
	$(LD) -dvSo $(ASMOBJ) $(COBJ) snesc.smc

%.s: %.c
ifeq ($(OPTIMIZE),1)
	$(CC) -I. -o $@.pre -c $<
	$(COPT) $@.pre > $@
	rm $@.pre
else
	$(CC) -I. -o $@ -c $<
endif

%.obj: %.s
	$(AS) -io $< $@
%.obj: %.asm
	$(AS) -io $< $@

clean:
	rm -f snesc.smc snesc.sym $(ASMOBJ) $(COBJ) *.s
