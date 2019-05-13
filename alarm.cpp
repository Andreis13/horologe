
#include "alarm.h"

Alarm::Alarm(Clock * clock) {
  m_clock = clock;
  m_hours = 0;
  m_minutes = 0;
}

bool Alarm::is_engaged() {
  if (is_matching()) {
    return !m_dismissed;
  } else {
    m_dismissed = false;
    return false;
  }
}

void Alarm::dismiss() {
  m_dismissed = true;
}

bool Alarm::is_matching() {
  m_clock->get_hours() == m_hours && m_clock->get_minutes() == m_minutes;
}

uint8_t Alarm::get_hours() {
  return m_hours;
}

uint8_t Alarm::get_minutes() {
  return m_minutes;
}

void Alarm::set_hours(uint8_t hours) {
  m_hours = hours;
}

void Alarm::set_minutes(uint8_t minutes) {
  m_minutes = minutes;
}

void Alarm::set(uint8_t hours, uint8_t minutes) {
  set_hours(hours);
  set_minutes(minutes);
}
