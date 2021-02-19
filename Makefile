NAME        := pam_party

prefix      ?= /usr/local
exec_prefix ?= $(prefix)
srcdir      ?= ./src
libdir      ?= $(exec_prefix)/lib

fname       := $(NAME).so
targetdir   := ./target
target      := $(targetdir)/$(fname)
destdir     := $(DESTDIR)$(libdir)/security

all: build

build:
	mkdir -p $(targetdir)
	gcc $(srcdir)/* -lpam -largon2 -shared -o $(target)

installdirs:
	install -d $(destdir)/

install: installdirs
	install -m 0644 $(target) $(destdir)/

uninstall:
	rm -f $(destdir)/$(fname)

clean:
	rm -rf $(targetdir)/
