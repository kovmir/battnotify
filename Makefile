PROJECT = battnotify
PREFIX ?= /usr/local
INSTALL ?= install

CC ?= cc

LIBNOTIFY_LIBS = `pkg-config --libs libnotify`
LIBNOTIFY_INCS = `pkg-config --cflags libnotify`

LIBS += ${LIBNOTIFY_LIBS}
INCS += ${LIBNOTIFY_INCS}

CFLAGS += -std=c99
CFLAGS += -pedantic
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wstrict-prototypes
CFLAGS += -O2
CFLAGS += ${INCS}

LDFLAGS += ${LIBS}

all:
	${CC} ./${PROJECT}.c ${CFLAGS} ${LDFLAGS} -o ./${PROJECT}

debug:
	${CC} ./${PROJECT}.c ${CFLAGS} -g ${LDFLAGS} -o ./${PROJECT}

gdb: debug
	gdb ./${PROJECT}

memcheck: debug
	valgrind --leak-check=yes ./${PROJECT}

memcheck_v: debug
	valgrind --leak-check=yes -v ./${PROJECT}

memcheck_full: debug
	valgrind --leak-check=full --show-leak-kinds=debug ./${PROJECT}

clean:
	rm -f ./${PROJECT}

install:
	mkdir -p "${DESTDIR}${PREFIX}/bin"
	${INSTALL} ./${PROJECT} "${DESTDIR}${PREFIX}/bin/${PROJECT}"

uninstall:
	rm -f "${DESTDIR}${PREFIX}/bin/${PROJECT}"
	rmdir --ignore-fail-on-non-empty "${DESTDIR}${PREFIX}/bin"

strip:
	strip ./${PROJECT}

.PHONY: all debug gdb memcheck memcheck_v memcheck_full clean install uninstall
