

#ifndef _HOROLOGE_STATE_H
#define _HOROLOGE_STATE_H

#include "Arduino.h"
#include "timeout.h"

class Horologe;

class State {
public:
  virtual State * update(uint32_t time) = 0;
  virtual void enter(uint32_t time) {};
  virtual void exit(uint32_t time) {};
};

class StandbyState : public State {
public:
  virtual State * update(uint32_t time);
  virtual void enter(uint32_t time);
};

class ActiveState : public State {
public:
  virtual State * update(uint32_t time);
  virtual void enter(uint32_t time);
private:
  virtual State * update_active(uint32_t time);
};

class SetupState : public ActiveState {
public:
  virtual void enter(uint32_t time);
  virtual void exit(uint32_t time);

private:
  virtual State * update_active(uint32_t time);
  virtual void commit();
  virtual void init_values();
  bool m_setting_hours;
protected:
  uint8_t m_hours;
  uint8_t m_minutes;
};

class AlarmSetupState : public SetupState {
private:
  virtual void commit();
  virtual void init_values();
};

class SetupMenuState : public ActiveState {
public:
  virtual void enter(uint32_t time);
private:
  virtual State * update_active(uint32_t time);
  uint8_t m_current_selection;
};

class States {
public:
  static StandbyState standby;
  static ActiveState active;
  static SetupState clock_setup;
  static AlarmSetupState alarm_setup;
  static SetupMenuState setup_menu;
};

#endif // _HOROLOGE_STATE_H
