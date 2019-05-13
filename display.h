#ifndef _HOROLOGE_DISPLAY_H
#define _HOROLOGE_DISPLAY_H

#include "Arduino.h"
#include "period.h"

class Display {
public:
  Display(uint8_t i2c_address = 0x70);
  void initialize();
  void set_brightness(uint8_t val);
  void set_blink(bool val);
  void set_blink_mask(uint8_t d1, uint8_t d2, uint8_t c, uint8_t d3, uint8_t d4);
  void update(uint32_t time);
  void draw_colon(bool should_draw);
  void draw_segments_raw(uint8_t mask, uint8_t position);
  void draw_dot(bool should_draw, uint8_t position);
  void draw_digit(uint8_t digit, uint8_t position);
  void draw_number(uint16_t number);
private:
  void write_buffer();
  void erase_display();
  uint8_t normalize_index(uint8_t index);

  uint8_t m_buffer[5];
  uint8_t m_previous_buffer[5];
  uint8_t m_blink_mask[5];
  bool m_blink_on;
  bool m_blink_phase_on;
  Period m_blink_period;
  uint8_t m_i2c_address;
};

#endif // _HOROLOGE_DISPLAY_H
