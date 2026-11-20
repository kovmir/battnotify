/* SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Ivan Kovmir */

#ifndef BATTERY_H
#define BATTERY_H
/* Library to query laptop battery capacity and charging state. */

/* Power sources. */
typedef enum {
	POWER_BATTERY, /* Not charging. */
	POWER_AC,      /* Charging. */
} PowerState;

/* Battery state. */
typedef struct {
	int        capacity; /* Battery capacity 0 to 100%. */
	PowerState power_state;
} BatteryInfo;

/* Battery handle. */
typedef struct {
	int cap_fd;
	int ac_fd;
} Battery;

/* Open (file descriptors) battery, must be called prior to query_battery(). */
int battery_open(const char *batt_path, const char *ac_path,
                 Battery *batt, const char **failed_path);
/* Query power state. */
int battery_query(Battery batt, BatteryInfo *bi);
/* Close (release file descriptors) battery when no longer needed. */
void battery_close(Battery *batt);

#endif /* BATTERY_H */
