PROJECT = battnotify
PREFIX ?= /usr/local
INSTALL ?= install

CC ?= cc

LIBNOTIFY_LIBS = $(shell pkg-config --libs libnotify)
LIBNOTIFY_INCS = $(shell pkg-config --cflags libnotify)

LIBS += $(LIBNOTIFY_LIBS)
INCS += $(LIBNOTIFY_INCS)

CFLAGS += -std=c23
CFLAGS += -pedantic
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wstrict-prototypes
CFLAGS += $(INCS)

LDFLAGS += $(LIBS)

build:
	$(CC) ./$(PROJECT).c $(CFLAGS) -O2 $(LDFLAGS) -o ./$(PROJECT)

debug:
	$(CC) ./$(PROJECT).c $(CFLAGS) -O0 -g $(LDFLAGS) -o ./$(PROJECT)

gdb: debug
	gdb ./$(PROJECT)

clean:
	rm -f ./$(PROJECT)

install:
	mkdir -p "$(DESTDIR)$(PREFIX)/bin"
	$(INSTALL) ./$(PROJECT) "$(DESTDIR)$(PREFIX)/bin/$(PROJECT)"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)/bin/$(PROJECT)"
	rmdir --ignore-fail-on-non-empty "$(DESTDIR)$(PREFIX)/bin"

strip:
	strip ./$(PROJECT)

.PHONY: build debug gdb clean install uninstall
