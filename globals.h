
#ifndef _GLOBALS_H
#define _GLOBALS_H

#include "Arduino.h"

#include "period.h"
#include "timeout.h"
#include "button.h"
#include "display.h"
#include "clock.h"
#include "alarm.h"
#include "led.h"

extern Display g_display;
extern Clock g_clock;
extern Alarm g_alarm;
extern Period g_p100ms;
extern Timeout g_ten_seconds;
extern Button g_mode_button;
extern Button g_set_button;
extern Led g_led;
extern bool g_must_sleep;

#endif // _GLOBALS_H
