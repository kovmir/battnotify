/* MIT License
 * Copyright 2024 Ivan Kovmir */

/* Includes */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <libnotify/notify.h>

/* Constants and Macros */
#define BUF_SIZE 16
#define AC_STATE_CHARGING 1
#define NTFN_APP_NAME "battnotify"

/* Function Prototypes */
/* Read battery charge percentage [0-100].
 * Return 1 on failure, 0 on success. */
static inline int get_batt_percentage(int *percent);
/* Return 1 if the device is charging, 0 otherwise. */
static inline int is_charging(int *state);
/* Read a string from file and parse an integer.
 * Return 1 on failure, 0 on success */
static inline int read_num_file(const char *file_path, int *num);

/* Global Variables */
#include "config.h"

inline int
read_num_file(const char *file_path, int *num)
{
	int fd;
	ssize_t n_read;
	char buf[BUF_SIZE];

	fd = open(file_path, O_RDONLY);
	if (fd < 0)
		return 1;

	n_read = read(fd, buf, BUF_SIZE);
	if (n_read < 0)
		return 1;
	buf[n_read] = 0;

	*num = (int)strtol(buf, NULL, 10);
	close(fd);
	return 0;
}

inline int
get_batt_percentage(int *percent)
{
	return read_num_file(batt_path, percent);
}

inline int
is_charging(int *state)
{
	return read_num_file(ac_path, state);
}

int
main(void)
{
	int batt_percent;   /* Battery charge level in percent. */
	int ac_status;      /* Power cord presence. */
	char buf[BUF_SIZE]; /* String to be sent to notification. */
	NotifyNotification *batt_ntfn; /* Notification handle. */

	/* Notification setup code. */
	notify_init(NTFN_APP_NAME);
	batt_ntfn = notify_notification_new(NULL, NULL, NULL);
	notify_notification_set_urgency(batt_ntfn, ntfn_urgency_level);
	notify_notification_set_timeout(batt_ntfn, ntfn_timeout);

	for (;; sleep(polling_delay)) {
		if (is_charging(&ac_status)) {
			fprintf(stderr,
				"failed to access %s\n", ac_path);
			return 1;
		}
		if (ac_status == AC_STATE_CHARGING)
			continue;

		if (get_batt_percentage(&batt_percent)) {
			fprintf(stderr, "failed to access %s\n", batt_path);
			return 1;
		}
		if (batt_percent > batt_warn_percent) {
			continue;
		}

		snprintf(buf, BUF_SIZE, "%d%%", batt_percent);

		notify_notification_update(batt_ntfn, ntfn_title, buf, NULL);
		notify_notification_show(batt_ntfn, NULL);
	}
}
