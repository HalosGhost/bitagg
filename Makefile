PROGNM =  bitagg
PREFIX ?= /usr/local
DOCDIR ?= $(DESTDIR)$(PREFIX)/share/man
LIBDIR ?= $(DESTDIR)$(PREFIX)/lib
BINDIR ?= $(DESTDIR)$(PREFIX)/bin
ZSHDIR ?= $(DESTDIR)$(PREFIX)/share/zsh
BSHDIR ?= $(DESTDIR)$(PREFIX)/share/bash-completions
MKDIR  ?= mkdir -p

include Makerules

.PHONY: all bin clean scan-build cov-build complexity doc verify install uninstall

all: dist bin

bin: dist
	@$(CC) $(CFLAGS) $(LDFLAGS) src/*.c -DVERSION="\"$(VER)\n\"" -o dist/$(PROGNM)

check: bin
	@./test-suite

clean:
	@rm -rf -- dist cov-int $(PROGNM).tgz make.sh ./src/*.plist

dist:
	@$(MKDIR) ./dist

doc: dist
	@(cd doc; \
		sphinx-build -b man -Dversion=$(VER) \
			-d doctree -E . ../dist $(PROGNM).rst; \
		rm -r -- doctree; \
	)

cov-build: dist
	@cov-build --dir cov-int ./make.sh
	@tar czvf $(PROGNM).tgz cov-int

complexity: bin
	complexity -h ./src/*

scan-build:
	@scan-build --use-cc=$(CC) make bin

verify:
	@frama-c $(FMFLAGS) src/*.c

install:
	@install -Dm755 dist/$(PROGNM) $(BINDIR)/$(PROGNM)

uninstall:
	@rm -f -- $(BINDIR)/$(PROGNM)
