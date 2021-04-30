NAME        := pam_party

prefix      ?= /usr/local
exec_prefix ?= $(prefix)
libdir      ?= $(exec_prefix)/lib

fname       := $(NAME).so
destdir     := $(DESTDIR)$(libdir)/security

all: build

build:
	cargo build --locked --release --lib

installdirs:
	install -d $(destdir)/

install: installdirs
	install -m 0644 ./target/release/lib$(fname) $(destdir)/$(fname)

uninstall:
	rm -f $(destdir)/$(fname)

clean:
	cargo clean
