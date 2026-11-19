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

/* Query power state. */
int get_battery(Battery batt, BatteryInfo *bi);

#endif /* BATTERY_H */
