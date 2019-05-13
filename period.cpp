
#include "period.h"

Period::Period(uint32_t interval) {
  m_interval = interval;
  m_last_tick_time = 0;
  m_has_ticked = false;
}

void Period::update(uint32_t time) {
  m_has_ticked = false;

  if (time > m_last_tick_time + m_interval) {
    m_has_ticked = true;
    m_last_tick_time = time;
  }
}

bool Period::elapsed() {
  return m_has_ticked;
}

void Period::set_interval(uint32_t interval) {
  m_interval = interval;
}

