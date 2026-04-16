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
CFLAGS += -DGIT_DESC=\"$(shell git describe --tags --always --dirty)\"
CFLAGS += $(INCS)

LDFLAGS += $(LIBS)

# Config values for debug build.
BATT_PATH     = ./batt_capacity
AC_PATH       = ./ac_plug
POLLING_DELAY = 10
WARN_PERCENT  = 15

build: CFLAGS += -DBUILD_TYPE=\"release\"
build:
	$(CC) ./$(PROJECT).c -O2 $(CFLAGS) $(LDFLAGS) -o ./$(PROJECT)

# No need for libnotify.
debug: LIBS :=
debug: CFLAGS += -DBUILD_TYPE=\"debug\"
debug:
	$(CC) \
		./$(PROJECT).c \
		-O0 -g $(CFLAGS) \
		-DDEBUG \
		-DDEBUG_BATT_PATH=\"$(BATT_PATH)\" \
		-DDEBUG_AC_PATH=\"$(AC_PATH)\" \
		-DDEBUG_POLLING_DELAY=$(POLLING_DELAY) \
		-DDEBUG_WARN_PERCENT=$(WARN_PERCENT) \
		$(LDFLAGS) \
		-o ./$(PROJECT)

# Run unit tests.
test: debug
	./test.sh $(BATT_PATH) $(AC_PATH)

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

.PHONY: build debug test gdb clean install uninstall
