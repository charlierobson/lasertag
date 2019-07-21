#ifndef __GUN_H
#define __GUN_H

extern unsigned long ledOffTime;

class Gun
{
protected:
  int _clipSize;
  int _ammoRemaining;
  uint16_t _triggerImpulse;

  const int IRLEDPIN = 11;
  const int TRIGGERPIN = 4;

  char shotBits[16];

public:
  Gun(GameConfig& gameConfig) :
      _clipSize(gameConfig.CLIPSIZE)
  {
    createShotBitstream(SHOT, (gameConfig.TEAMID & 3) << 4 | gameConfig.PLAYERID & 15, shotBits);
  }

  void begin() {
    pinMode(TRIGGERPIN, INPUT_PULLUP);

    _ammoRemaining = _clipSize;
    _triggerImpulse = 0;

    // we toggle this pin between input and output to control
    // transmission of the 38khz IR pulses
    pinMode (IRLEDPIN, INPUT);

    // set up Timer 2 for a (good enough near) 38khz rate
    // 0C2A pin is toggled on compare match, in this case arduino PIN11
    TCCR2A = _BV (COM2A0) | _BV(WGM21);
    TCCR2B = _BV (CS20);
    OCR2A =  209;
  }

  // gun subclasses must supply this function
  virtual void update() = 0;

protected:
    bool triggerPulled() {
      _triggerImpulse <<= 1;
      if (digitalRead(TRIGGERPIN) == LOW)
        _triggerImpulse |= 1;

      return _triggerImpulse == 0x7fff;
    }

  void fire(char* ordnanceBits) {
    // 2400us start bit
    pinMode(IRLEDPIN, OUTPUT);
    delayMicroseconds(2400);
    pinMode(IRLEDPIN, INPUT);
    delayMicroseconds(600); // shorten this?

    // command, payload and parity
    for (int i = 0; i < 16; ++i) {
      int pulseLen = ordnanceBits[i] * 600;
      pinMode(IRLEDPIN, OUTPUT);
      delayMicroseconds(pulseLen);
      pinMode(IRLEDPIN, INPUT);
      delayMicroseconds(600);
    }

    ledOffTime = millis() + 100;
    digitalWrite(5,HIGH);
    Serial.println("bang!");
 
//    --_ammoRemaining;
  }

  uint16_t createShotBitstream(int cmd, int payload, char* destBuffer) {
    // for parity calculation
    int oneBits = 0;

    // form the shot payload from a 6 bit command and 9 bit payload with a parity check
    // the payload contains a stream of 16 * 600us pulse lengths where a value of:
    //   2  represents a '1' bit, of 2 * 600us pulses
    //   1  represents a '0' bit, of 1 * 600us pulse

    uint16_t shot = (cmd & 63) << 10 | (payload & 511) << 1;

    for (int i = 0; i < 15; ++i) {
      if (shot & 0x8000) {
        *destBuffer = 2;
        ++oneBits;
      } else {
        *destBuffer = 1;
      }
      shot <<= 1;
      ++destBuffer;
    }

    // parity calculation. always an even number of bits.
    *destBuffer = oneBits & 1 ? 2 : 1;
  }
};

#endif
