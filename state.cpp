
#include "state.h"
#include "globals.h"

StandbyState States::standby;
ActiveState States::active;
SetupState States::clock_setup;
AlarmSetupState States::alarm_setup;
SetupMenuState States::setup_menu;

void StandbyState::enter(uint32_t time) {
  g_display.set_brightness(0);
}

State * StandbyState::update(uint32_t time) {
  if (g_mode_button.was_pressed() || g_set_button.was_pressed())
  {
    return &States::active;
  }

  g_display.draw_colon(true);
  g_display.draw_number(g_clock.get_hours() * 100 + g_clock.get_minutes());

  g_must_sleep = true;
  return NULL;
}

void ActiveState::enter(uint32_t time) {
  g_ten_seconds.reset(time);
  g_display.set_brightness(15);
}

State * ActiveState::update(uint32_t time) {
  g_ten_seconds.update(time);

  if (g_mode_button.was_pressed() || g_set_button.was_pressed()) {
    g_ten_seconds.reset(time);
  }

  if (g_ten_seconds.elapsed()) {
    return &States::standby;
  }

  return update_active(time);
}

State * ActiveState::update_active(uint32_t time) {
  if (g_mode_button.was_pressed())
  {
    return &States::setup_menu;
  }

  g_display.draw_colon(true);
  g_display.draw_number(g_clock.get_hours() * 100 + g_clock.get_minutes());

  return NULL;
}


void SetupState::enter(uint32_t time) {
  init_values();

  m_setting_hours = true;

  g_display.set_blink(true);
  g_display.set_blink_mask(0xFF, 0xFF, 0x00, 0x00, 0x00);
}

void SetupState::init_values() {
  m_hours = g_clock.get_hours();
  m_minutes = g_clock.get_minutes();
}

State * SetupState::update_active(uint32_t time) {
  if (g_set_button.was_pressed())
  {
    if (m_setting_hours) {
      m_hours = (m_hours + 1) % 24;
      commit();
    } else {
      m_minutes = (m_minutes + 1) % 60;
      commit();
    }
  }

  if (g_mode_button.was_pressed())
  {
    if (m_setting_hours) {
      m_setting_hours = false;
      g_display.set_blink_mask(0x00, 0x00, 0x00, 0xFF, 0xFF);
    } else {
      return &States::active;
    }
  }

  g_display.draw_colon(true);
  g_display.draw_number(m_hours * 100 + m_minutes);

  return NULL;
}

void SetupState::commit() {
  g_clock.set(m_hours, m_minutes);
}

void SetupState::exit(uint32_t time) {
  g_display.set_blink(false);
}

void AlarmSetupState::commit() {
  g_alarm.set(m_hours, m_minutes);
}

void AlarmSetupState::init_values() {
  m_hours = g_alarm.get_hours();
  m_minutes = g_alarm.get_minutes();
}


void SetupMenuState::enter(uint32_t time) {
  m_current_selection = 0;
  g_display.draw_colon(false);
}

State * SetupMenuState::update_active(uint32_t time) {
  if (g_mode_button.was_pressed()) {
    m_current_selection++;
    if (m_current_selection > 1) {
      return &States::active;
    }
  }

  if (g_set_button.was_pressed()) {
    switch(m_current_selection) {
      case 0:
        return &States::clock_setup;
      case 1:
        return &States::alarm_setup;
    }
  }

  switch(m_current_selection) {
    case 0:
      g_display.draw_segments_raw(0x40, 0); // -
      g_display.draw_segments_raw(0x39, 1); // C
      g_display.draw_segments_raw(0x38, 2); // L
      g_display.draw_segments_raw(0x40, 3); // -
      break;
    case 1:
      g_display.draw_segments_raw(0x40, 0); // -
      g_display.draw_segments_raw(0x77, 1); // A
      g_display.draw_segments_raw(0x38, 2); // L
      g_display.draw_segments_raw(0x40, 3); // -
      break;
    default:
      break;
  }

  return NULL;
}
