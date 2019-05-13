
#include "globals.h"
#include "state.h"

#define MODE_BTN_PIN 4
#define SET_BTN_PIN 3
#define LED_PIN 13

Display g_display;
Clock g_clock;
Alarm g_alarm = Alarm(&g_clock);
Period g_p100ms = Period(100000);
Timeout g_ten_seconds = Timeout(10000000);
Button g_mode_button = Button(MODE_BTN_PIN);
Button g_set_button = Button(SET_BTN_PIN);
Led g_led = Led(LED_PIN);
State * g_state;
bool g_must_sleep = false;


void setup() {
  Serial.begin(9600);

  g_led.initialize();
  g_mode_button.initialize();
  g_set_button.initialize();
  g_clock.initialize();
  g_display.initialize();

  g_state = &States::standby;
  g_state->enter(millis());

  enable_interrupts();

  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
}

void loop() {

  uint32_t time = micros();

  g_mode_button.update(time);
  g_set_button.update(time);
  g_p100ms.update(time);
  g_clock.update(time);

  State * new_state = g_state->update(time);

  if (new_state)
  {
    g_state->exit(time);
    g_state = new_state;
    g_state->enter(time);
  }

  // audio_player.update(time);
  g_display.update(time);

  if (g_must_sleep) {
    g_must_sleep = false;
    __DSB();
    __WFI();
  }
}

void enable_interrupts() {
  attachInterrupt(digitalPinToInterrupt(MODE_BTN_PIN), dummy_function, RISING);
  attachInterrupt(digitalPinToInterrupt(SET_BTN_PIN), dummy_function, RISING);

  GCLK->CLKCTRL.reg = (uint16_t)(
    GCLK_CLKCTRL_GEN_GCLK2 |        // Select Generic Clock Generator 2 (same as RTC)
    GCLK_CLKCTRL_ID(EIC_GCLK_ID) |  // Select Generic Clock for EIC
    GCLK_CLKCTRL_CLKEN              // Enable clock
  );

  g_clock.enable_rtc_interrupt(dummy_function);
}

void dummy_function() { }



