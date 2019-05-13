
#include "led.h"

Led::Led(uint8_t pin) {
  m_pin = pin;
}

void Led::initialize() {
  pinMode(m_pin, OUTPUT);
  off();
}

void Led::on() {
  digitalWrite(m_pin, HIGH);
  m_is_on = true;
}

void Led::off() {
  digitalWrite(m_pin, LOW);
  m_is_on = false;
}

void Led::toggle() {
  if (m_is_on) {
    off();
  } else {
    on();
  }
}
