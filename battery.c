/* SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Ivan Kovmir */
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#include "battery.h"

int read_int(int *out_buf, int fd);

int
read_int(int *out_buf, int fd)
{
	char *endp;
	size_t n_read;
	char buf[32];
	long int value;

	/* Read the file. */
	n_read = read(fd, buf, sizeof(buf)-1);
	if (n_read <= 0)
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
get_battery(BatteryInfo *bi, int cap_fd, int ac_fd)
{
	int tmp;

	assert(bi != NULL);

	if (read_int(&tmp, cap_fd) != 0)
		return -1;
	bi->capacity = tmp;
	if (read_int(&tmp, ac_fd) != 0)
		return -1;
	bi->power_state = (tmp == 1) ? POWER_AC : POWER_BATTERY;
	return 0;
}
