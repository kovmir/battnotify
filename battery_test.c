/* SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Ivan Kovmir */
#ifdef NDEBUG
#error "Enable assert() for unit tests."
#endif
#include <assert.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "battery.h"

#define MKSTEMP_TEMPLATE "/tmp/mindwmstat_XXXXXX"

static void test_get_load(void);
static void test_get_time(void);
static void test_get_battery(void);
static void test_get_ram(void);

void
test_get_load(void)
{
	char buf[16];
	float a, b, c;
	int n_matches;

	memset(buf, 0xAA, sizeof(buf));
	assert(get_load(buf, sizeof(buf)) == 0);
	n_matches = sscanf(buf, "%f %f %f", &a, &b, &c);

	assert(n_matches == 3);
	assert(strlen(buf) < sizeof(buf));
}

void
test_get_time(void)
{
	char buf[64];

	setenv("TZ", "UTC", 1);
	tzset();
	memset(buf, 0xAA, sizeof(buf));
	assert(get_time(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", 0) == 0);
	assert(strcmp(buf, "1970-01-01 00:00:00") == 0);
	assert(strlen(buf) < sizeof(buf));
}

void
test_get_battery(void)
{
	BatteryInfo bi;
	char cap_content[] = "42";
	char ac_content[] = "1";
	char cap_tmpl[] = MKSTEMP_TEMPLATE;
	char ac_tmpl[] = MKSTEMP_TEMPLATE;
	int cap_fd = mkstemp(cap_tmpl);
	int ac_fd = mkstemp(ac_tmpl);
	assert(cap_fd != -1);
	assert(ac_fd != -1);
	/* Write test content. */
	assert(write(cap_fd, cap_content, strlen(cap_content)) != -1);
	assert(write(ac_fd, ac_content, strlen(ac_content)) != -1);
	lseek(cap_fd, 0, SEEK_SET);
	lseek(ac_fd, 0, SEEK_SET);
	/* Read battery status. */
	assert(get_battery(&bi, cap_fd, ac_fd) == 0);
	assert(bi.capacity == 42);
	assert(bi.power_state == POWER_AC);
	/* Clean up. */
	assert(close(cap_fd) == 0);
	assert(close(ac_fd) == 0);
	assert(unlink(cap_tmpl) == 0);
	assert(unlink(ac_tmpl) == 0);
}

void
test_get_ram(void)
{
	char meminfo_buf[] = "MemTotal: 100 kB\nWhatEver: 666TB\nMemAvailable: 50 kB\n";
	FILE *meminfo_file = fmemopen(meminfo_buf, sizeof(meminfo_buf), "r");
	int used_mem;

	assert(get_memory(&used_mem, meminfo_file) == 0);
	assert(used_mem = 50);

	assert(fclose(meminfo_file) == 0);
}

int
main(void)
{
	test_get_load();
	test_get_time();
	test_get_battery();
	test_get_ram();
	return 0;
}
