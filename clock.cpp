
#include "clock.h"

Clock::Clock() {
  m_minutes = 0;
  m_hours = 0;
}

void (*Clock::rtc_interrupt_handler)(void) = NULL;

void Clock::initialize() {
  PM->APBAMASK.reg |= PM_APBAMASK_RTC;

  init_clock_system();
  configure_rtc();
}

void Clock::update(uint32_t time) {
  request_read_rtc();
  RTC_MODE2_CLOCK_Type rtc_time;
  rtc_time.reg = RTC->MODE2.CLOCK.reg;
  m_minutes = rtc_time.bit.MINUTE;
  m_hours = rtc_time.bit.HOUR;
}

uint8_t Clock::get_hours() {
  return m_hours;
}

uint8_t Clock::get_minutes() {
  return m_minutes;
}

void Clock::set(uint8_t hours, uint8_t minutes) {
  set_hours(hours);
  set_minutes(minutes);
}

void Clock::set_hours(uint8_t hours) {
  RTC->MODE2.CLOCK.bit.HOUR = hours; // TODO: Change to HOUR
  sync_rtc();
  m_hours = hours;
}

void Clock::set_minutes(uint8_t minutes) {
  RTC->MODE2.CLOCK.bit.MINUTE = minutes; // TODO: Change to MINUTE
  sync_rtc();
  m_minutes = minutes;
}

void Clock::enable_rtc() {
  RTC->MODE2.CTRL.reg |= RTC_MODE2_CTRL_ENABLE;
  sync_rtc();
}

void Clock::disable_rtc() {
  RTC->MODE2.CTRL.reg &= ~RTC_MODE2_CTRL_ENABLE;
  sync_rtc();
}

void Clock::reset_rtc() {
  RTC->MODE2.CTRL.reg |= RTC_MODE2_CTRL_SWRST;
  sync_rtc();
}

void Clock::remove_reset_rtc() {
  RTC->MODE2.CTRL.reg &= ~RTC_MODE2_CTRL_SWRST;
  sync_rtc();
}

void Clock::sync_rtc() {
  while(RTC->MODE2.STATUS.bit.SYNCBUSY)
    ;
}

void Clock::init_clock_system() {
  // ENABLING A PERIPHERAL
  // In order to enable a peripheral that is clocked by a Generic Clock, the following parts of the system needs
  // to be configured:
  // • A running Clock Source.
  // • A clock from the Generic Clock Generator must be configured to use one of the running Clock
  // Sources, and the Generator must be enabled.
  // • The Generic Clock Multiplexer that provides the Generic Clock signal to the peripheral must be
  // configured to use a running Generic Clock Generator, and the Generic Clock must be enabled.
  // • The user interface of the peripheral needs to be unmasked in the PM. If this is not done the
  // peripheral registers will read all 0’s and any writing attempts to the peripheral will be discarded.

  // wait for internal oscillator to be ready for setup
  while(!SYSCTRL->PCLKSR.bit.OSC32KRDY)
    ;

  // fetch factory calibration values
  uint32_t calib_value = (*((uint32_t*)NVMCTRL_OTP4 + 1) >> 6) & 0x7F;

  // Enable clock source (internal oscillator)
  SYSCTRL->OSC32K.reg = SYSCTRL_OSC32K_ONDEMAND |
                         SYSCTRL_OSC32K_RUNSTDBY |
                         SYSCTRL_OSC32K_EN32K |
                         SYSCTRL_OSC32K_STARTUP(6) |
                         SYSCTRL_OSC32K_CALIB(calib_value) |
                         SYSCTRL_OSC32K_ENABLE;



  // Set divison factor 32 for GCLK 2 to obtain a 1 kHz clock
  // Divison factor will be calculated as 2^(GENDIV.DIV+1) => 2^(4+1) = 32
  // because GENCTRL.DIVSEL is set to 1 later
  GCLK->GENDIV.reg = GCLK_GENDIV_ID(2) | GCLK_GENDIV_DIV(4);
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;

  // Enable Generic Clock Generator
  GCLK->GENCTRL.reg = (
    GCLK_GENCTRL_ID(2) |      // Select Generic Clock Generator 2
    GCLK_GENCTRL_GENEN |      // Enable Generic Clock Generator
    GCLK_GENCTRL_SRC_OSC32K | // Select internal oscillator as clock source
    GCLK_GENCTRL_DIVSEL
  );
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;

  // Configure Generic Clock
  GCLK->CLKCTRL.reg = (uint16_t)(
    GCLK_CLKCTRL_GEN_GCLK2 |        // Select Generic Clock Generator 2
    GCLK_CLKCTRL_ID(RTC_GCLK_ID) |  // Select Generic Clock for RTC
    GCLK_CLKCTRL_CLKEN              // Enable clock
  );
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;
}

void Clock::configure_rtc() {
  disable_rtc();
  reset_rtc();

  uint16_t tmp_reg = 0;

  tmp_reg |= RTC_MODE2_CTRL_MODE_CLOCK;         // set clock operating mode
  tmp_reg |= RTC_MODE2_CTRL_PRESCALER_DIV1024;  // set prescaler to 1024 for MODE2
  tmp_reg &= ~RTC_MODE2_CTRL_MATCHCLR;          // disable clear on match
  tmp_reg &= ~RTC_MODE2_CTRL_CLKREP;            // 24h time representation

  RTC->MODE2.READREQ.reg &= ~RTC_READREQ_RCONT; // disable continuously mode

  RTC->MODE2.CTRL.reg = tmp_reg;
  sync_rtc();

  remove_reset_rtc();
  enable_rtc();
}

void Clock::request_read_rtc() {
  RTC->MODE2.READREQ.reg = RTC_READREQ_RREQ;
  sync_rtc();
}

void Clock::enable_rtc_interrupt(void (*handler)(void)) {
  Clock::rtc_interrupt_handler = handler;

  NVIC_EnableIRQ(RTC_IRQn); // enable RTC interrupt
  NVIC_SetPriority(RTC_IRQn, 0x00);

  RTC->MODE2.INTENSET.reg |= RTC_MODE2_INTENSET_ALARM0; // enable alarm interrupt
  RTC->MODE2.Mode2Alarm[0].MASK.bit.SEL = RTC_MODE2_MASK_SEL_SS;
  sync_rtc();
}

void RTC_Handler(void) {
  if (Clock::rtc_interrupt_handler) {
    Clock::rtc_interrupt_handler();
  }
  RTC->MODE2.INTFLAG.reg = RTC_MODE2_INTFLAG_ALARM0;
}
