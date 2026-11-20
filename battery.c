/* SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Ivan Kovmir */
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "battery.h"

static int read_int(int *out_buf, int fd);

static int
read_int(int *out_buf, int fd)
{
	char *endp;
	size_t n_read;
	char buf[32];
	long int value;
	ssize_t r;

	assert(out_buf != NULL);
	assert(fd >= 0);

	/* Read the file. Loop to handle short reads and EINTR. */
	n_read = 0;
	while (n_read < sizeof(buf)-1) {
		r = read(fd, buf + n_read, sizeof(buf)-1 - n_read);
		if (r == -1) {
			if (errno == EINTR)
				continue; /* Interrupted: retry. */
			return -1;
		}
		if (r == 0)
			break; /* EOF. */
		n_read += (size_t)r;
	}
	if (n_read == 0)
		return -1;
	buf[n_read] = '\0';

	/* Parse integer from the line. */
	errno = 0;
	value = strtol(buf, &endp, 10);

	/* Error checking... */
	if (buf == endp)
		return -1; /* No integer found. */
	if (errno != 0)
		return -1; /* Parse error / strtol overflow. */
	if (value > INT_MAX || value < INT_MIN)
		return -1; /* Integer overflow — fits in long but not int. */
	/* Reject trailing garbage. "123abc" should not silently parse as 123. */
	while (*endp != '\0') {
		if (*endp != ' ' && *endp != '\t' &&
		    *endp != '\n' && *endp != '\r')
			return -1; /* Trailing garbage. */
		endp++;
	}

	/* Good. */
	*out_buf = value;
	return 0;
}

int
open_battery(const char *batt_path, const char *ac_path,
             Battery *batt, const char **failed_path)
{
	int tmp;

	assert(batt_path != NULL);
	assert(ac_path != NULL);
	assert(batt != NULL);

	batt->cap_fd = -1;
	batt->ac_fd = -1;
	tmp = open(batt_path, O_RDONLY);
	if (tmp == -1) {
		if (failed_path != NULL)
			*failed_path = batt_path;
		return -1;
	}
	batt->cap_fd = tmp;
	tmp = open(ac_path, O_RDONLY);
	if (tmp == -1) {
		if (failed_path != NULL)
			*failed_path = ac_path;
		close(batt->cap_fd);
		return -1;
	}
	batt->ac_fd = tmp;
	return 0;
}

int
query_battery(Battery batt, BatteryInfo *bi)
{
	int tmp;

	assert(batt.cap_fd >= 0);
	assert(batt.ac_fd >= 0);

	if (lseek(batt.cap_fd, 0, SEEK_SET) ||
	    lseek(batt.ac_fd, 0, SEEK_SET))
		return -1;
	if (read_int(&tmp, batt.cap_fd) != 0)
		return -1;
	bi->capacity = tmp;
	if (read_int(&tmp, batt.ac_fd) != 0)
		return -1;
	bi->power_state = (tmp == 1) ? POWER_AC : POWER_BATTERY;
	return 0;
}

void
close_battery(Battery *batt)
{
	assert(batt != NULL);
	if (batt->cap_fd != -1) {
		close(batt->cap_fd);
		batt->cap_fd = -1;
	}
	if (batt->ac_fd != -1) {
		close(batt->ac_fd);
		batt->ac_fd = -1;
	}
}
