CC ?= cc

LIBNOTIFY_CFLAGS := $(shell pkg-config --cflags libnotify)
LIBNOTIFY_LDLIBS := $(shell pkg-config --libs libnotify)
GIT_VERSION := $(shell git describe --tags --always --dirty 2>/dev/null)

CFLAGS += -std=c99
CFLAGS += -g
CFLAGS += -O2
CFLAGS += -pedantic
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -D_DEFAULT_SOURCE

ifneq ($(GIT_VERSION),)
CFLAGS += -DGIT_VERSION='"$(GIT_VERSION)"'
endif

BUILD_DIR := build
BIN_DIR   := $(BUILD_DIR)/bin
SRC_DIR   := src
TEST_DIR  := test

BUILD_BIN := $(BIN_DIR)/battnotify
TEST_BIN  := $(BIN_DIR)/battery_test

LIB_SRCS := $(SRC_DIR)/notify.c $(SRC_DIR)/battery.c
LIB_OBJS := $(LIB_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
MAIN_OBJ := $(BUILD_DIR)/main.o
TEST_OBJ := $(BUILD_DIR)/battery_test.o

DEPS := $(MAIN_OBJ:.o=.d) $(LIB_OBJS:.o=.d) $(TEST_OBJ:.o=.d)

PREFIX ?= /usr/local

all: build

build: $(BUILD_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

$(BUILD_BIN): CFLAGS += $(LIBNOTIFY_CFLAGS)
$(BUILD_BIN): LDLIBS += $(LIBNOTIFY_LDLIBS)
$(BUILD_BIN): $(MAIN_OBJ) $(LIB_OBJS)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(TEST_BIN): CPPFLAGS += -I$(SRC_DIR)
$(TEST_BIN): $(TEST_OBJ) $(BUILD_DIR)/battery.o
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c -o $@ $<

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c -o $@ $<

install:
	install -Dm755 $(BUILD_BIN) "$(DESTDIR)$(PREFIX)/bin/$(notdir $(BUILD_BIN))"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)/bin/$(BUILD_BIN)"

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)

.PHONY: all build test install uninstall clean
