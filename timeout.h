
#ifndef _HOROLOGE_TIMEOUT_H
#define _HOROLOGE_TIMEOUT_H

#include "Arduino.h"

class Timeout {
public:
  Timeout(uint32_t interval);
  void update(uint32_t time);
  void reset(uint32_t time);
  bool elapsed();
  void set_interval(uint32_t interval);
private:

  uint32_t m_interval;
  uint32_t m_reset_time;
  uint32_t m_current_time;
};

#endif // _HOROLOGE_TIMEOUT_H
