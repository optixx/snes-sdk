SNES9X=1
PREFIX = /usr/local/snes-sdk

SUBDIRS = wla_dx wla_dx/wlalink tcc-65816 libs
ifeq ($(SNES9X),1)
SUBDIRS += snes9x
endif

all: $(SUBDIRS)

.PHONY: dummy $(SUBDIRS)
libs wla_dx snes9x wla_dx/wlalink: dummy
	cd $@ && $(MAKE)
tcc-65816: tcc-65816/config.h
	@cd $@ && $(MAKE) 816-tcc

tcc-65816/config.h:
	cd tcc-65816 && ./configure --prefix=$(PREFIX) --build-cross
snes9x: snes9x/config.info
snes9x/config.info:
	cd snes9x --prefix=$(PREFIX) && ./configure --with-extra-opt="-Wno-parentheses -Wno-missing-braces" --with-debugger

libs: wla_dx

clean:
	(for i in $(SUBDIRS) ; do $(MAKE) -C $$i clean || true ; done)

distclean: clean
	cd snes9x && rm -f conftezt.out.2 Makefile dependencies config.status config.log config.info snes9x
	cd snesc && $(MAKE) clean

install:
	mkdir -p $(DESTDIR)$(PREFIX)
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -p libs/*.obj $(DESTDIR)$(PREFIX)/lib/
	cp -p libs/hdr.asm $(DESTDIR)$(PREFIX)/include/
	cp -p tcc-65816/include/* $(DESTDIR)$(PREFIX)/include/
	cp -p tcc-65816/816-tcc $(DESTDIR)$(PREFIX)/bin/
	cp -p wla_dx/wla-65816 $(DESTDIR)$(PREFIX)/bin/
	cp -p wla_dx/wlalink/wlalink $(DESTDIR)$(PREFIX)/bin/
ifeq ($(SNES9X),1)
	cp -p snes9x/snes9x $(DESTDIR)$(PREFIX)/bin/
endif
