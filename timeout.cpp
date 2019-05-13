
#include "timeout.h"

Timeout::Timeout(uint32_t interval) {
  m_interval = interval;
  m_reset_time = 0;
  m_current_time = 0;
}

void Timeout::update(uint32_t time) {
  m_current_time = time;
}

void Timeout::reset(uint32_t time) {
  m_reset_time = time;
}

bool Timeout::elapsed() {
  return m_current_time > (m_reset_time + m_interval);
}

void Timeout::set_interval(uint32_t interval) {
  m_interval = interval;
}
