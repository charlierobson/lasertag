#ifndef __GUN_H
#define __GUN_H

#include "timerMacros.h"


class Gun
{
  public:
    Gun(GameConfig& gameConfig) :
      _clipSize(gameConfig.CLIPSIZE)
    {
      createShotBitstream(SHOT, (gameConfig.TEAMID & 3) << 4 | gameConfig.PLAYERID & 15, _shotBits);
    }

    void begin() {
      pinMode(TRIGGERPIN, INPUT_PULLUP);

      _ammoRemaining = _clipSize;
      _triggerImpulse = 0;

      pwmEnable(false);

      TIMER_INITPWM;
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
      pwmEnable(true);
      delayMicroseconds(2400);
      pwmEnable(false);
      delayMicroseconds(600); // shorten this?

      // command, payload and parity
      for (int i = 0; i < 16; ++i) {
        int pulseLen = ordnanceBits[i] * 600;
        pwmEnable(true);
        delayMicroseconds(pulseLen);
        pwmEnable(false);
        delayMicroseconds(600);
      }

      //    --_ammoRemaining;
    }

    uint16_t createShotBitstream(int cmd, int payload, char* destBuffer) {
      // form the shot payload from a 6 bit command and 9 bit payload with a parity check
      // the payload contains a stream of 16 * 600us pulse lengths where a value of:
      //   2  represents a '1' bit, of 2 * 600us pulses
      //   1  represents a '0' bit, of 1 * 600us pulse

      uint16_t shot = (cmd & 63) << 10 | (payload & 511) << 1;

      // for parity calculation
      int oneBits = 0;

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

    int _clipSize;
    int _ammoRemaining;
    uint16_t _triggerImpulse;

    char _shotBits[16];

    const int IRLEDPIN = 11;
    const int TRIGGERPIN = 4;

#ifdef ESP32

    void pwmEnable(bool on) {
    }

#else

    void pwmEnable(bool on) {
      if (on) pinMode(IRLEDPIN, OUTPUT);
      else    pinMode(IRLEDPIN, INPUT);
    }

#endif
};

#endif
