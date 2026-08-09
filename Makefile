CC ?= cc

LIBNOTIFY_CFLAGS := $(shell pkg-config --cflags libnotify)
LIBNOTIFY_LDLIBS := $(shell pkg-config --libs libnotify)
GIT_VERSION := $(shell git describe --tags --always --dirty)

CFLAGS += -std=c99
CFLAGS += -g
CFLAGS += -O2
CFLAGS += -pedantic
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -D_DEFAULT_SOURCE
CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

BUILD_BIN := battnotify
TEST_BIN  := stats_test

PREFIX ?= /usr/local

all: build

build: $(BUILD_BIN)

test: $(TEST_BIN)

$(BUILD_BIN): CFLAGS += $(LIBNOTIFY_CFLAGS)
$(BUILD_BIN): LDLIBS += $(LIBNOTIFY_LDLIBS)
$(BUILD_BIN): main.o stats.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(TEST_BIN): stats_test.o stats.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

main.o: config.h stats.h

stats.o: stats.h

install:
	install -Dm755 ./$(BUILD_BIN) "$(DESTDIR)$(PREFIX)/bin/$(BUILD_BIN)"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)/bin/$(BUILD_BIN)"

clean:
	rm -f *.o $(BUILD_BIN) $(TEST_BIN)

.PHONY: all build test install uninstall clean
