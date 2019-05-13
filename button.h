
#ifndef _HOROLOGE_BUTTON_H
#define _HOROLOGE_BUTTON_H

#include "Arduino.h"

class Button {
public:
  Button(uint8_t pin, bool pressed_on_high = true);
  void initialize();
  bool is_down();
  bool is_up();
  bool was_pressed();
  bool was_clicked();
  bool was_released();
  void update(uint32_t time);
private:
  bool m_currently_pressed;
  bool m_previously_pressed;
  uint8_t m_pin;
  bool m_pressed_on_high;
};

#endif // _HOROLOGE_BUTTON_H
