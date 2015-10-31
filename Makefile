.PHONY: all clean distclean dist lib doc examples selftest bootloader

all: lib doc examples bootloader

clean:
	$(MAKE) -C libtinyasuro clean
	$(MAKE) -C doc clean
	$(MAKE) -C examples clean
	$(MAKE) -C tinyasuroboot clean
	rm -f TinyAsuroLib.zip

distclean:
	$(MAKE) -C libtinyasuro distclean
	$(MAKE) -C doc distclean
	$(MAKE) -C examples distclean
	$(MAKE) -C tinyasuroboot distclean

dist: TinyAsuroLib.zip

lib: 
	$(MAKE) -C libtinyasuro

doc:
	$(MAKE) -C doc

examples:
	$(MAKE) -C examples

selftest:
	$(MAKE) -C examples/SelfTest

bootloader:
	$(MAKE) -C tinyasuroboot

TinyAsuroLib.zip: clean lib doc selftest bootloader distclean
	zip -r -q $@ *
