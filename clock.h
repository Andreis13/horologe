
#ifndef _HOROLOGE_CLOCK_H
#define _HOROLOGE_CLOCK_H

#include "Arduino.h"

class Clock {
public:
  Clock();
  void initialize();

  bool alarm_engaged();
  void dismiss_alarm();

  void update(uint32_t time);

  uint8_t get_hours();
  uint8_t get_minutes();

  void set(uint8_t hours, uint8_t minutes);
  void set_hours(uint8_t hours);
  void set_minutes(uint8_t minutes);

  void enable_rtc_interrupt(void (*handler)(void));

  static void (*rtc_interrupt_handler)(void);

private:
  void enable_rtc();
  void disable_rtc();
  void reset_rtc();
  void remove_reset_rtc();
  void sync_rtc();
  void init_clock_system();
  void configure_rtc();
  void request_read_rtc();

  uint8_t m_minutes;
  uint8_t m_hours;
};

#endif // _HOROLOGE_CLOCK_H
