/* SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Ivan Kovmir */
#ifdef NDEBUG
#error "Enable assert() for unit tests."
#endif
#include <assert.h>

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "battery.h"

#define MKSTEMP_TEMPLATE "/tmp/battnotify_XXXXXX"

static void test_battery_close(void);
static void test_battery_open(void);
static void test_battery_query(void);

void
test_battery_close(void)
{
	Battery batt;
	int cap_fd = open("/dev/null", O_RDONLY);
	int ac_fd = open("/dev/null", O_RDONLY);
	assert(cap_fd != -1);
	assert(ac_fd != -1);

	/* Unopened handle: nothing to close. */
	batt.cap_fd = -1;
	batt.ac_fd = -1;
	battery_close(&batt);
	assert(batt.cap_fd == -1);
	assert(batt.ac_fd == -1);
	/* Only cap_fd valid: ac_fd is left untouched. */
	batt.cap_fd = cap_fd;
	batt.ac_fd = -1;
	battery_close(&batt);
	assert(batt.cap_fd == -1);
	assert(batt.ac_fd == -1);
	assert(close(cap_fd) == -1 && errno == EBADF);
	/* Only ac_fd valid: cap_fd is left untouched. */
	batt.cap_fd = -1;
	batt.ac_fd = ac_fd;
	battery_close(&batt);
	assert(batt.cap_fd == -1);
	assert(batt.ac_fd == -1);
	assert(close(ac_fd) == -1 && errno == EBADF);
	/* Both fds valid: both closed, double close is safe. */
	cap_fd = open("/dev/null", O_RDONLY);
	ac_fd = open("/dev/null", O_RDONLY);
	assert(cap_fd != -1);
	assert(ac_fd != -1);
	batt.cap_fd = cap_fd;
	batt.ac_fd = ac_fd;
	battery_close(&batt);
	assert(batt.cap_fd == -1);
	assert(batt.ac_fd == -1);
	assert(close(cap_fd) == -1 && errno == EBADF);
	assert(close(ac_fd) == -1 && errno == EBADF);
	/* Double close: no-op. */
	battery_close(&batt);
	assert(batt.cap_fd == -1);
	assert(batt.ac_fd == -1);
}

void
test_battery_open(void)
{
	Battery batt;
	const char *bad_ac = "/nonexistent/online";
	const char *bad_cap = "/nonexistent/capacity";
	const char *failed_path = NULL;
	char cap_tmpl[] = MKSTEMP_TEMPLATE;
	char ac_tmpl[] = MKSTEMP_TEMPLATE;
	int fd;

	/* Create the backing files. */
	fd = mkstemp(cap_tmpl);
	assert(fd != -1);
	assert(close(fd) == 0);
	fd = mkstemp(ac_tmpl);
	assert(fd != -1);
	assert(close(fd) == 0);
	/* Success: both paths exist. */
	assert(battery_open(cap_tmpl, ac_tmpl, &batt, &failed_path) == 0);
	assert(failed_path == NULL); /* Untouched on success. */
	assert(batt.cap_fd != -1);
	assert(batt.ac_fd != -1);
	battery_close(&batt);
	assert(batt.cap_fd == -1);
	assert(batt.ac_fd == -1);
	/* Failure: missing battery path. */
	failed_path = NULL;
	assert(battery_open(bad_cap, ac_tmpl, &batt, &failed_path) == -1);
	assert(failed_path == bad_cap);
	/* Failure: NULL failed_path is accepted. */
	assert(battery_open(bad_cap, ac_tmpl, &batt, NULL) == -1);
	/* Failure: missing AC path. */
	failed_path = NULL;
	assert(battery_open(cap_tmpl, bad_ac, &batt, &failed_path) == -1);
	assert(failed_path == bad_ac);
	/* Clean up. */
	assert(unlink(cap_tmpl) == 0);
	assert(unlink(ac_tmpl) == 0);
}

void
test_battery_query(void)
{
	Battery batt;
	BatteryInfo bi;
	char cap_content[] = "42";
	char ac_content[] = "1";
	char cap_tmpl[] = MKSTEMP_TEMPLATE;
	char ac_tmpl[] = MKSTEMP_TEMPLATE;
	int cap_fd = mkstemp(cap_tmpl);
	int ac_fd = mkstemp(ac_tmpl);
	assert(cap_fd != -1);
	assert(ac_fd != -1);
	batt.cap_fd = cap_fd;
	batt.ac_fd = ac_fd;
	/* Write test content. */
	assert(write(cap_fd, cap_content, strlen(cap_content)) != -1);
	assert(write(ac_fd, ac_content, strlen(ac_content)) != -1);
	/* Read battery status. */
	assert(battery_query(batt, &bi) == 0);
	assert(bi.capacity == 42);
	assert(bi.power_state == POWER_AC);
	/* Read again... */
	assert(battery_query(batt, &bi) == 0);
	assert(bi.capacity == 42);
	assert(bi.power_state == POWER_AC);
	/* Clean up. */
	assert(close(cap_fd) == 0);
	assert(close(ac_fd) == 0);
	assert(unlink(cap_tmpl) == 0);
	assert(unlink(ac_tmpl) == 0);
}

int
main(void)
{
	test_battery_open();
	test_battery_query();
	test_battery_close();
	return 0;
}
