#ifndef __GUN_H
#define __GUN_H

class Gun
{
protected:
  int _clipSize;
  int _ammoRemaining;
  bool _lastTriggerPinActive;

  const int PIN_TRIGGER = 4;

public:
  Gun(GameConfig& gameConfig) :
      _clipSize(gameConfig.CLIPSIZE)
  {
  }

  void begin() {
    pinMode(PIN_TRIGGER, INPUT_PULLUP);

    _ammoRemaining = _clipSize;
    _lastTriggerPinActive = false;
  }

  // gun subclasses must supply this function
  virtual void update() = 0;

protected:
  bool triggerPulled() {
    bool triggerPinActive = digitalRead(PIN_TRIGGER) == LOW;
    bool triggered = triggerPinActive && !_lastTriggerPinActive;

    _lastTriggerPinActive = triggerPinActive;

    return triggered;
  }

  void fire() {
    
    --_ammoRemaining;
  }
};

#endif
