#pragma once

#include "TimerMacros.h"
#include "PWMMacros.h"

class Gun
{
  public:
    Gun(GameConfig& gameConfig, int irLEDPin) :
      _clipSize(gameConfig.CLIPSIZE),
      _trigger(trigger),
      _irLEDPin(irLEDPin)
    {
      createShotBitstream(SHOT, (gameConfig.TEAMID & 3) << 4 | gameConfig.PLAYERID & 15, _shotBits);
    }

    void begin() {
      PWM_INIT;
      PWM_DISABLE;
    }

    // gun subclasses must supply this function
    virtual void update() = 0;

  protected:
    void fire(char* ordnanceBits) {

      // 2400us start bit
      PWM_ENABLE;
      delayMicroseconds(2400);
      PWM_DISABLE;
      delayMicroseconds(600); // shorten this?

      // command, payload and parity
      for (int i = 0; i < 16; ++i) {
        int pulseLen = ordnanceBits[i] * 600;
        PWM_ENABLE;
        delayMicroseconds(pulseLen);
        PWM_DISABLE;
        delayMicroseconds(600);
      }
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

    int _irLEDPin;

    char _shotBits[16];
};
