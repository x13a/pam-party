NAME        := pam_party

prefix      ?= /usr/local
exec_prefix ?= $(prefix)
libdir      ?= $(exec_prefix)/lib

fname       := $(NAME).so
destdir     := $(DESTDIR)$(libdir)/security
targetdir   := ./zig-out

all: build

build:
	zig build

installdirs:
	install -d $(destdir)/

install: installdirs
	install -m 0600 $(targetdir)/lib/lib$(fname) $(destdir)/$(fname)

uninstall:
	rm -f $(destdir)/$(fname)

clean:
	rm -rf ./zig-cache/
	rm -rf $(targetdir)/
