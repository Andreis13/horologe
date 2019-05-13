
#ifndef _HOROLOGE_ALARM_H
#define _HOROLOGE_ALARM_H

#include "Arduino.h"
#include "clock.h"

class Alarm {
public:
  Alarm(Clock * clock);
  bool is_engaged();
  void dismiss();
  bool is_matching();
  uint8_t get_hours();
  uint8_t get_minutes();
  void set_hours(uint8_t hours);
  void set_minutes(uint8_t minutes);
  void set(uint8_t hours, uint8_t minutes);

private:
  Clock * m_clock;
  uint8_t m_hours;
  uint8_t m_minutes;
  bool m_dismissed;
};

#endif // _HOROLOGE_ALARM_H
