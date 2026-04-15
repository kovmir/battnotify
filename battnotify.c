/* SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Ivan Kovmir */

/* Includes */
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <libnotify/notify.h>

#ifndef __linux__
    #error "Only Linux is supported."
#endif

/* Constants and Macros */
#define MESSAGE_LEN 16
#define AC_STATE_CHARGING 1
#define NTFN_APP_NAME "battnotify"

/* Function Prototypes */
/* Get current battery capacity percentage; return true on success. */
static inline bool get_charge(int *percent);
/* Get AC power plug state; return true on success. */
static bool is_charging(bool *state);
/* Parse an integer from the first line of the file; return true on success. */
static bool read_num_file(const char *file_path, int *num);

/* Global Variables */
#include "config.h"

#ifdef DEBUG
/* Battery to read state from. */
static const char *batt_path = DEBUG_BATT_PATH;
/* AC/DC adapter to read state from. */
static const char *ac_path = DEBUG_AC_PATH;
#endif /* DEBUG */

bool
read_num_file(const char *file_path, int *num)
{
	FILE *file;
	const int BUF_SIZE = 16;
	char  buf[BUF_SIZE];
	char *endp;

	/* Read the first line from the file. */
	errno = 0;
	file = fopen(file_path, "r");
	if (file == NULL) {
		warn("unable to open %s", file_path);
		return false;
	}
	if (fgets(buf, BUF_SIZE, file) == NULL) {
		warnx("unable to read from %s", file_path);
		fclose(file);
		return false;
	}
	fclose(file);

	/* Parse integer from the line. */
	errno = 0;
	*num = (int)strtol(buf, &endp, 10);
	if (errno != 0) {
		warn("unable to parse integer from %s", file_path);
		return false;
	}
	if (buf == endp) {
		warnx("no integer found in %s", file_path);
		return false;
	}
	return true;
}

inline bool
get_charge(int *batt_charge)
{
	return read_num_file(batt_path, batt_charge);
}

bool
is_charging(bool *ac_state)
{
	int i;
	bool ok;

	ok = read_num_file(ac_path, &i);
	*ac_state = i == AC_STATE_CHARGING ? true : false;
	return ok;
}

[[noreturn]] int
main(int argc, char *argv[])
{
	char msg[MESSAGE_LEN]; /* String to be sent to notification. */
	int  batt_charge;
	bool charging;
	bool ok;

	(void)argv; /* Suppress -Wunused-parameter. */
	if (argc > 1) {
		puts(GIT_DESC); /* Print version. */
		return 0;
	}

	NotifyNotification *batt_ntfn; /* Notification handle. */

	/* Notification init code. */
	notify_init(NTFN_APP_NAME);
	batt_ntfn = notify_notification_new(NULL, NULL, NULL);
	notify_notification_set_urgency(batt_ntfn, ntfn_urgency_level);
	notify_notification_set_timeout(batt_ntfn, ntfn_timeout);

	for (;; sleep(polling_delay)) {
		ok = is_charging(&charging);
		if (ok == false)
			errx(1, "unable to get charging status");

		if (charging == true) {
#ifdef DEBUG
			exit(0);
#else
			continue; /* Charging? Nevermind. */
#endif
		}

		ok = get_charge(&batt_charge);
		if (ok == false)
			errx(1, "unable to get battery charge");

		if (batt_charge > batt_warn_percent) {
#ifdef DEBUG
			exit(0);
#else
			continue; /* Not charging, and no need for it. */
#endif
		}

		/* Not charging and the battery is low. */
		snprintf(msg, MESSAGE_LEN, "%d%%", batt_charge);
#ifdef DEBUG
		(void)ntfn_title; /* Suppres -Wunused-variable. */
		puts(msg);
		exit(0);
#else
		notify_notification_update(batt_ntfn, ntfn_title, msg, NULL);
		notify_notification_show(batt_ntfn, NULL);
#endif
	}
}
