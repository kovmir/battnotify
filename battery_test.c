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

static void test_get_battery(void);

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

int
main(void)
{
	test_get_battery();
	return 0;
}
