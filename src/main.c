/* SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Ivan Kovmir */
#include <assert.h>
#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "battery.h"
#include "notify.h"

#ifndef __linux__
#error "Unsupported operating system."
#endif /* __linux__ */

#define SUPPORT_URL "https://github.com/kovmir/battnotify/issues"
#ifndef GIT_VERSION
#define GIT_VERSION "dev"
#endif /* GIT_VERSION */

#include "config.h"


int
main(int argc, char *argv[])
{
	int notified = 0;
	BatteryInfo bi;
	Battery batt;
	NotifyNotification *ntfn;
	const char *failed_path = NULL;

	(void)argv; /* Suppress -Wunused-parameter. */
	if (argc > 1) {
		printf("%s\n", GIT_VERSION);
		printf("Support: %s\n", SUPPORT_URL);
		return 0;
	}

	if (open_battery(batt_path, ac_path, &batt, &failed_path) == -1)
		err(1, "unable to open %s", failed_path);

	ntfn = notify_open(ntfn_app_name, ntfn_urgency_level, ntfn_timeout);
	if (ntfn == NULL)
		errx(1, "unable to initialize libnotify");

	assert(polling_delay > 0);
	for (;; sleep(polling_delay)) {
		if (query_battery(batt, &bi) == -1) {
			warnx("unable to query battery info");
			continue;
		}
		if (bi.power_state == POWER_AC) {
			notified = 0;
			continue;
		}
		if (bi.capacity > batt_warn_percent)
			continue;

		/* Not charging and low battery capacity... */

		if (notified == 1)
			continue; /* Notification has already been sent. */

		/* Send notification. */
		if (notify_send(ntfn, ntfn_title, bi.capacity) == -1) {
			warnx("unable to send notification");
			continue;
		}
		notified = 1;
	}
	return 0;
}
