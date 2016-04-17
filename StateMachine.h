#pragma once
//Simple state machine class
#include <Arduino.h>

class StateM;

typedef void *(SMFunc)(StateM* sm);

class StateM
{
public:
  static const uint8_t STATE_INVALID = 255;

  void init(SMFunc* handlerFunc)
  {
    handlerFunc_ = handlerFunc;
    current_ = STATE_INVALID;
    previous_ = STATE_INVALID;
    run();
  }

  inline void setNext(uint8_t next)
  {
    next_ = next;
  }

  uint8_t run()
  {
    uint8_t retVal;

    next_ = STATE_INVALID;
    retVal = handlerFunc_(this);
    previous_ = current_;
    if( STATE_INVALID != next_ )
    {
      previous_ = current_;
      current_ = next_;
      Serial.print("Switched to state ");
      Serial.println((int)current_);
      enterTime_ = millis();
    }
    return retVal;
  }

  //How long we've been in this state
  unsigned long getDuration() const
  {
    return millis() - enterTime_;
  }

  bool isStateEntered() const
  {
    return current_ != previous_;
  }

  uint8_t getCurrent() const
  {
    return current_;
  }

private:
  uint8_t current_;
  uint8_t previous_;
  uint8_t next_;
  unsigned long enterTime_;
  SMFunc* handlerFunc_;
};
