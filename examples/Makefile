EXAMPLE_DIRS = $(filter-out Makefile, $(wildcard *))
MAKE_DIRS = $(addprefix make_, $(EXAMPLE_DIRS))
CLEAN_DIRS = $(addprefix clean_, $(EXAMPLE_DIRS))
DISTCLEAN_DIRS = $(addprefix distclean_, $(filter-out SelfTest, $(EXAMPLE_DIRS)))

.PRECIOUS: $(EXAMPLE_DIRS)
.PHONY: all clean distclean $(MAKE_DIRS) $(CLEAN_DIRS) $(DISTCLEAN_DIRS)

all: $(MAKE_DIRS)

clean: $(CLEAN_DIRS)

distclean: $(DISTCLEAN_DIRS)
	$(MAKE) -C SelfTest distclean

$(MAKE_DIRS): make_%:
	$(MAKE) -C $* all
	
$(CLEAN_DIRS): clean_%:
	$(MAKE) -C $* clean

$(DISTCLEAN_DIRS): distclean_%:
	$(MAKE) -C $* clean
