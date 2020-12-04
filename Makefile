NAME      := pam_party

srcdir    ?= ./src

fname     := $(NAME).so
targetdir := ./target
target    := $(targetdir)/$(fname)
destdir   := $(DESTDIR)/lib/security

all: build

build:
	gcc $(srcdir)/* -lpam -lcrypto -shared -o $(target)

installdirs:
	install -o root -g root -d $(destdir)/

install: installdirs
	install -m 0644 -o root -g root $(target) $(destdir)/

uninstall:
	rm -f $(destdir)/$(fname)

clean:
	rm -rf $(targetdir)/
