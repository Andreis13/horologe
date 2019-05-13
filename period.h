
#ifndef _HOROLOGE_PERIOD_H
#define _HOROLOGE_PERIOD_H

#include "Arduino.h"

class Period {
public:
  Period(uint32_t interval);
  void update(uint32_t time);
  bool elapsed();
  void set_interval(uint32_t interval);

private:
  uint32_t m_interval;
  uint32_t m_last_tick_time;
  bool m_has_ticked;
};

#endif // _HOROLOGE_PERIOD_H
