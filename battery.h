/* SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Ivan Kovmir */
#ifndef BATTERY_H
#define BATTERY_H

#include <stdio.h>
#include <time.h>

typedef enum {
	POWER_BATTERY,
	POWER_AC,
} PowerState;

typedef struct {
	int        capacity;
	PowerState power_state;
} BatteryInfo;

/* All functions return -1 on failure, and 0 on success. */

/* Return system load in /porc/loadavg fashion. */
int get_load(char *out_buf, size_t out_len);
/* Get current system date & time. */
int get_time(char *out_buf, size_t out_len, const char *format, time_t t);
/* Get battery charge and charging status. */
int get_battery(BatteryInfo *bi, int cap_fd, int ac_fd);
/* Get occupied RAM percentage. */
int get_memory(int *out_buf, FILE *meminfo);

#endif /* BATTERY_H */
