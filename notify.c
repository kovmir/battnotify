/* SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Ivan Kovmir */
#include <assert.h>

#include "notify.h"

NotifyNotification *
notify_open(const char *app, NotifyUrgency urgency, gint timeout)
{
	NotifyNotification *n;

	assert(app != NULL);

	if (notify_init(app) != TRUE)
		return NULL;
	n = notify_notification_new(NULL, NULL, NULL);
	if (n == NULL)
		return NULL;
	notify_notification_set_urgency(n, urgency);
	notify_notification_set_timeout(n, timeout);
	return n;
}

int
notify_send(NotifyNotification *ntfn, const char *title, int capacity)
{
	char msg[NOTIFY_MESSAGE_LEN]; /* Notification message. */

	assert(ntfn != NULL);
	assert(title != NULL);

	snprintf(msg, NOTIFY_MESSAGE_LEN, "%d%%", capacity);
	if (notify_notification_update(ntfn, title, msg, NULL) != TRUE)
		return -1;
	if (notify_notification_show(ntfn, NULL) != TRUE)
		return -1;
	return 0;
}

void
notify_close(NotifyNotification *ntfn)
{
	assert(ntfn != NULL);

	notify_notification_close(ntfn, NULL);
	g_object_unref(ntfn);
	notify_uninit();
}
