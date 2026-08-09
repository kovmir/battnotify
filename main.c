#include <assert.h>
#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include <libnotify/notify.h>

#include "stats.h"

#ifndef __linux__
#error "Unsupported operating system."
#endif /* __linux__ */

#define SUPPORT_URL "https://github.com/kovmir/battnotify/issues"
#ifndef GIT_VERSION
#define GIT_VERSION "dev"
#endif /* GIT_VERSION */
#define MESSAGE_LEN 16

#include "config.h"

int
main(int argc, char *argv[])
{
	char msg[MESSAGE_LEN]; /* String to be sent to notification. */
	int notified = 0;
	NotifyNotification *ntfn;
	BatteryInfo bi;
	int cap_fd, ac_fd;

	(void)argv; /* Suppress -Wunused-parameter. */
	if (argc > 1) {
		printf("%s\n", GIT_VERSION);
		printf("Support: %s\n", SUPPORT_URL);
		return 0;
	}

	cap_fd = open(batt_path, O_RDONLY);
	if (cap_fd == -1)
		err(1, "battery not found %s", batt_path);
	ac_fd = open(ac_path, O_RDONLY);
	if (ac_fd == -1)
		err(1, "AC plug not found %s", ac_path);

	notify_init(ntfn_app_name);
	ntfn = notify_notification_new(NULL, NULL, NULL);
	notify_notification_set_urgency(ntfn, ntfn_urgency_level);
	notify_notification_set_timeout(ntfn, ntfn_timeout);

	assert(polling_delay > 0);
	for (;; sleep(polling_delay)) {
		if (get_battery(&bi, cap_fd, ac_fd) == -1)
			errx(1, "unable to get battery info");

		if (bi.power_state == POWER_AC) {
			notified = 0;
			continue; /* Charging. */
		}

		/* Not charging... */
		if (bi.capacity > batt_warn_percent)
			continue; /* Enough battery capacity. */

		/* Low battery capacity... */
		if (notified == 1)
			continue; /* Notification has already been sent. */

		snprintf(msg, MESSAGE_LEN, "%d%%", bi.capacity);
		notify_notification_update(ntfn, ntfn_title, msg, NULL);
		notify_notification_show(ntfn, NULL);

		notified = 1;
		lseek(cap_fd, 0, SEEK_SET);
		lseek(ac_fd, 0, SEEK_SET);
	}
	return 0;
}
