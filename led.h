
#ifndef _HOROLOGE_LED_H
#define _HOROLOGE_LED_H

#include "Arduino.h"

class Led {
public:
  Led(uint8_t pin);
  void initialize();
  void on();
  void off();
  void toggle();
private:
  uint8_t m_pin;
  bool m_is_on;
};

#endif // _HOROLOGE_LED_H
