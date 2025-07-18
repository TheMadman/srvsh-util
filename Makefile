TARGETS=pipeline filter

INSTALL_PROGRAM?=install
prefix?=/usr/local
bindir?=/bin
LDLIBS=-lsrvsh

all: $(TARGETS)

.PHONY: install install-% clean

install: $(TARGETS:%=install-%)

install-%: % | $(prefix)$(bindir)
	$(INSTALL_PROGRAM) $< $(prefix)$(bindir)/$<

clean:
	rm -f $(TARGETS)

$(prefix)$(bindir):
	mkdir -p $@
