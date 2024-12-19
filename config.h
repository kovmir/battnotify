/* Battery to read state from. */
static const char *batt_path = "/sys/class/power_supply/BAT0/capacity";
/* AC/DC adapter to read state from. */
static const char *ac_path = "/sys/class/power_supply/AC/online";
/* Battery/AC polling rate in seconds. */
static const int polling_delay = 10;
/* Send a notification when battery level is below this. */
static const int batt_warn_percent = 15;

/* Set notification urgency level.
 * NOTIFY_URGENCY_LOW
 * NOTIFY_URGENCY_NORMAL
 * NOTIFY_URGENCY_CRITICAL */
static const NotifyUrgency ntfn_urgency_level = NOTIFY_URGENCY_CRITICAL;
/* Set notification timeout in milliseconds.
 * Or you can use one of the following constants:
 * NOTIFY_EXPIRES_DEFAULT
 * NOTIFY_EXPIRES_NEVER */
static const gint ntfn_timeout = NOTIFY_EXPIRES_DEFAULT;
/* Notification title. */
static const char *ntfn_title = "Low Battery Level";
