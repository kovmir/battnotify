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

# Fake battery and AC power plug files for debug.
BATT_CAP = ./batt_capacity
AC_PLUG  = ./ac_plug

build:
	$(CC) ./$(PROJECT).c $(CFLAGS) -O2 $(LDFLAGS) -o ./$(PROJECT)

debug:
	$(CC) \
		./$(PROJECT).c \
		$(CFLAGS) -O0 -g \
		-DDEBUG_PATHS \
		-DDEBUG_BATT_PATH=\"$(BATT_CAP)\" \
		-DDEBUG_AC_PATH=\"$(AC_PLUG)\" \
		$(LDFLAGS) \
		-o ./$(PROJECT)

batt:
	echo 14 > $(BATT_CAP)
	echo 0  > $(AC_PLUG)

gdb: debug
	gdb ./$(PROJECT)

clean:
	rm -f ./$(PROJECT) $(BATT_CAP) $(AC_PLUG)

install:
	mkdir -p "$(DESTDIR)$(PREFIX)/bin"
	$(INSTALL) ./$(PROJECT) "$(DESTDIR)$(PREFIX)/bin/$(PROJECT)"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)/bin/$(PROJECT)"
	rmdir --ignore-fail-on-non-empty "$(DESTDIR)$(PREFIX)/bin"

strip:
	strip ./$(PROJECT)

.PHONY: build debug gdb clean install uninstall
