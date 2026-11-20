/* SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Ivan Kovmir */
#ifndef NOTIFY_H
#define NOTIFY_H

#include <libnotify/notify.h>

#define NOTIFY_MESSAGE_LEN 16

NotifyNotification *notify_open(const char *app, NotifyUrgency urgency, gint timeout);
int notify_send(NotifyNotification *ntfn, const char *title, int capacity);
void notify_close(NotifyNotification *ntfn);

#endif /* NOTIFY_H */
