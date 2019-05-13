
#include "button.h"

Button::Button(uint8_t pin, bool pressed_on_high) {
  m_pin = pin;
  m_pressed_on_high = pressed_on_high;
  m_currently_pressed = false;
  m_previously_pressed = false;
}

void Button::initialize() {
  pinMode(m_pin, INPUT);
}

bool Button::is_down() {
  return m_currently_pressed;
}

bool Button::is_up() {
  return !m_currently_pressed;
}

bool Button::was_pressed() {
  return !m_previously_pressed && m_currently_pressed;
}

bool Button::was_clicked() {
  return false; // not implemented
}

bool Button::was_released() {
  return m_previously_pressed && !m_currently_pressed;
}

void Button::update(uint32_t time) {
  m_previously_pressed = m_currently_pressed;
  m_currently_pressed = digitalRead(m_pin) == HIGH && m_pressed_on_high;
}

