#include <assert.h>
#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include <libnotify/notify.h>

#include "battery.h"

#ifndef __linux__
#error "Unsupported operating system."
#endif /* __linux__ */

#define SUPPORT_URL "https://github.com/kovmir/battnotify/issues"
#ifndef GIT_VERSION
#define GIT_VERSION "dev"
#endif /* GIT_VERSION */
#define MESSAGE_LEN 16

#include "config.h"

static NotifyNotification *init_notification(void);
static int send_notification(NotifyNotification *n, int capacity);

static NotifyNotification *
init_notification(void)
{
	NotifyNotification *n;
	if (notify_init(ntfn_app_name) != TRUE)
		return NULL;
	n = notify_notification_new(NULL, NULL, NULL);
	if (n == NULL)
		return NULL;
	notify_notification_set_urgency(n, ntfn_urgency_level);
	notify_notification_set_timeout(n, ntfn_timeout);
	return n;
}

static int
send_notification(NotifyNotification *n, int capacity)
{
	char msg[MESSAGE_LEN]; /* Notification message. */
	snprintf(msg, MESSAGE_LEN, "%d%%", capacity);
	if (notify_notification_update(n, ntfn_title, msg, NULL) != TRUE)
		return -1;
	if (notify_notification_show(n, NULL) != TRUE)
		return -1;
	return 0;
}

int
main(int argc, char *argv[])
{
	int notified = 0;
	BatteryInfo bi;
	int cap_fd, ac_fd;
	NotifyNotification *ntfn;

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

	ntfn = init_notification();
	if (ntfn == NULL)
		errx(1, "unable to initialize libnotify");

	assert(polling_delay > 0);
	for (;; sleep(polling_delay)) {
		lseek(cap_fd, 0, SEEK_SET);
		lseek(ac_fd, 0, SEEK_SET);
		if (get_battery(&bi, cap_fd, ac_fd) == -1) {
			warnx("unable to get battery info");
			continue;
		}

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

		/* Send notification. */
		if (send_notification(ntfn, bi.capacity) == -1) {
			warnx("unable to send notification");
			continue;
		}
		notified = 1;
	}
	return 0;
}
