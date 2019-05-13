
#include "display.h"

#include <Wire.h>

#define HT16K33_SYS_SETUP_OSCILLATOR_ON 0x21
#define HT16K33_SYS_SETUP_OSCILLATOR_OFF 0x20
#define HT16K33_DISPLAY_SETUP_CMD 0x80
#define HT16K33_DISPLAY_SETUP_DISPLAY_ON 0x01
#define HT16K33_BRIGHTNESS(val) (0xE0 | (val & 0x0F))

static const uint8_t number_table[] = {
  0x3F, /* 0 */
  0x06, /* 1 */
  0x5B, /* 2 */
  0x4F, /* 3 */
  0x66, /* 4 */
  0x6D, /* 5 */
  0x7D, /* 6 */
  0x07, /* 7 */
  0x7F, /* 8 */
  0x6F, /* 9 */
  0x77, /* A */
  0x7C, /* b */
  0x39, /* C */
  0x5E, /* d */
  0x79, /* E */
  0x71, /* F */
};

Display::Display(uint8_t i2c_address) :
  m_blink_period(500000) // half a second
{
  m_i2c_address = i2c_address;
  memset(m_previous_buffer, 0, 5);
  memset(m_buffer, 0, 5);
  memset(m_blink_mask, 0, 5);
  m_blink_on = false;
  m_blink_phase_on = false;
}

void Display::initialize() {
  Wire.begin();

  Wire.beginTransmission(m_i2c_address);
  Wire.write(HT16K33_SYS_SETUP_OSCILLATOR_ON);
  Wire.endTransmission();

  Wire.beginTransmission(m_i2c_address);
  Wire.write(HT16K33_DISPLAY_SETUP_CMD | HT16K33_DISPLAY_SETUP_DISPLAY_ON);
  Wire.endTransmission();


  set_brightness(15);

  erase_display();

  write_buffer();
}

void Display::set_brightness(uint8_t val) {
  if (val > 15) {
    val = 15;
  }
  Wire.beginTransmission(m_i2c_address);
  Wire.write(HT16K33_BRIGHTNESS(val));
  Wire.endTransmission();
}

void Display::set_blink(bool val) {
  m_blink_on = val;
}

void Display::set_blink_mask(uint8_t d1, uint8_t d2, uint8_t c, uint8_t d3, uint8_t d4) {
  m_blink_mask[0] = d1;
  m_blink_mask[1] = d2;
  m_blink_mask[2] = c;
  m_blink_mask[3] = d3;
  m_blink_mask[4] = d4;
}

void Display::update(uint32_t time) {
  if (m_blink_on) {
    m_blink_period.update(time);
    if (m_blink_period.elapsed()) {
      m_blink_phase_on = !m_blink_phase_on;
    }
  }
  write_buffer();
}

void Display::draw_colon(bool should_draw) {
  if (should_draw) {
    m_buffer[2] = 0x02;
  } else {
    m_buffer[2] = 0x00;
  }
}

void Display::draw_segments_raw(uint8_t mask, uint8_t position) {
  m_buffer[normalize_index(position)] = mask;
}

void Display::draw_dot(bool should_draw, uint8_t position) {
  if (should_draw) {
    m_buffer[normalize_index(position)] |= 0x80;
  } else {
    m_buffer[normalize_index(position)] &= 0x7F;
  }
}

void Display::draw_digit(uint8_t digit, uint8_t position) {
  draw_segments_raw(number_table[digit % 10], position);
}

void Display::draw_number(uint16_t number) {
  number %= 10000;
  draw_digit(number / 1000, 0);
  draw_digit(number % 1000 / 100, 1);
  draw_digit(number % 100 / 10, 2);
  draw_digit(number % 10, 3);
}

uint8_t Display::normalize_index(uint8_t index) {
  switch(index) {
    case 0:
    case 1:
      return index;
    case 2:
    case 3:
      return index + 1;
    default:
      return 4;
  }
}

void Display::write_buffer() {
  uint8_t to_be_displayed[5];

  memcpy(to_be_displayed, m_buffer, 5);

  if (m_blink_on && m_blink_phase_on) {
    for (int i = 0; i < 5; ++i)
    {
      to_be_displayed[i] &= ~m_blink_mask[i];
    }
  }

  if (memcmp(m_previous_buffer, to_be_displayed, 5) == 0) {
    return; // buffers are equal, nothing to do
  }

  memcpy(m_previous_buffer, to_be_displayed, 5);

  Wire.beginTransmission(m_i2c_address);
  Wire.write((uint8_t)0x00); // start at address $00

  for (uint8_t i=0; i<5; i++) {
    Wire.write(to_be_displayed[i]);
    Wire.write((uint8_t)0x00);
  }

  Wire.endTransmission();
}

void Display::erase_display() {
  Wire.beginTransmission(m_i2c_address);

  Wire.write((uint8_t)0x00); // start at address $00

  for (uint8_t i=0; i<16; i++) {
    Wire.write((uint8_t)0x00);
  }

  Wire.endTransmission();
}

