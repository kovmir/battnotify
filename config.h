/* Battery to read state from. */
static const char *BATT_PATH = "/sys/class/power_supply/BAT0/capacity";
/* AC/DC adapter to read state from. */
static const char *AC_PATH = "/sys/class/power_supply/AC/online";
/* Battery/AC polling rate in seconds. */
static const int DELAY = 10;
/* Send a notification when battery level is below this. */
static const int BATT_WARN_PERCENT = 15;

/* Set notification urgency level.
 * NOTIFY_URGENCY_LOW
 * NOTIFY_URGENCY_NORMAL
 * NOTIFY_URGENCY_CRITICAL */
static const NotifyUrgency URGENCY_LEVEL = NOTIFY_URGENCY_CRITICAL;
/* Set notification timeout in milliseconds.
 * Or you can use one of the following constants:
 * NOTIFY_EXPIRES_DEFAULT
 * NOTIFY_EXPIRES_NEVER */
static const gint TIMEOUT = NOTIFY_EXPIRES_DEFAULT;
