/* ISC License
 * Copyright 2023 Ivan Kovmir */

/* Includes */
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <libnotify/notify.h>

/* Constants and Macros */
#define BUF_SIZE 16

#define AC_OFF 0
#define NOTIFY_APP_NAME "battnotify"

/* Global Variables */
#include "config.h"

/* Function Prototypes */
/* Returns battery charge percentage [0-100]. */
static int get_batt_percentage(void);
/* Returns AC/DC plug state. 1 if plugged in, and 0 otherwise. */
static int get_ac_status(void);
/* Returns an integer read from a given file. */
static inline int safe_read_num(const char *file_path);

inline int
safe_read_num(const char *file_path)
{
	int fd;
	ssize_t n_read;
	char buf[BUF_SIZE];
	fd = open(file_path, O_RDONLY);
	if (fd < 0)
		err(1, "failed to open %s", file_path);
	n_read = read(fd, buf, BUF_SIZE);
	if (n_read < 0)
		err(1, "failed to read %s", file_path);
	buf[n_read] = '\0';
	close(fd);
	return (int)strtol(buf, NULL, 10);
}

int
get_batt_percentage(void)
{
	return safe_read_num(BATT_PATH);
}

int
get_ac_status(void)
{
	return safe_read_num(AC_PATH);
}

int
main(void)
{
	int batt_percent;
	int ac_status;
	char buf[BUF_SIZE];
	NotifyNotification *batt_notifcn;

	notify_init(NOTIFY_APP_NAME);
	batt_notifcn = notify_notification_new(NULL, NULL, NULL);
	notify_notification_set_urgency(batt_notifcn, URGENCY_LEVEL);
	notify_notification_set_timeout(batt_notifcn, TIMEOUT);

	for (;; sleep(DELAY)) {
		batt_percent = get_batt_percentage();
		if (batt_percent < BATT_WARN_PERCENT) {
			ac_status = get_ac_status();
			if (ac_status == AC_OFF) {
				snprintf(buf, BUF_SIZE, "%d%%", batt_percent);
				notify_notification_update(batt_notifcn,
					"Low Battery Level", buf, NULL);
				notify_notification_show(batt_notifcn, NULL);
			}
		}
	}
}
