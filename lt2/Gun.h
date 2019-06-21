class Gun
{
private:
  int _clipSize;
  int _ammoRemaining;
  bool _lastTriggerPinActive;

  const int PIN_TRIGGER = 4;
  
public:
  Gun(int clipSize) :
      _clipSize(clipSize)
  {
  }

  void begin() {
    pinMode(PIN_TRIGGER, INPUT_PULLUP);

    _ammoRemaining = _clipSize;
    _lastTriggerPinActive = false;
  }

  void update() {
    if (!_ammoRemaining) return;
    if (!triggerPulled()) return;

    fire();
  }

private:
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
