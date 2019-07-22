#pragma once

#include "TimerMacros.h"
#include "RingBuffer.h"

// forward declarations

extern void indicatorOn(unsigned int duration);

class Receiver* iamReceiver;
void irIsr();


// IR receiver class

class Receiver
{
  public:
    Receiver(int receiverPin) :
      _receiverPin(receiverPin) {
      _ringBuffer = new RingBuffer();
    }

    void begin() {
      iamReceiver = this;
      attachInterrupt(digitalPinToInterrupt(_receiverPin), irIsr, CHANGE);
      TIMER_INIT;
    }

    void update() {
    }

    bool detectedHit() {
      if (!_ringBuffer->dataPresent()) return false;

      uint16_t data = _ringBuffer->pop();

      uint16_t command = data >> 10;
      uint16_t payload = (data & 1023) >> 1;

      if (command == SHOT) {
        uint16_t team = payload >> 4;
        uint16_t player = payload & 15;
        Serial.print("shot by player ");
        Serial.print(player, DEC);
        Serial.print(" of team ");
        Serial.println(team, DEC);

        indicatorOn(250);
      }

      return true;
    }

    void handleInterrupt() {
      if (digitalRead(_receiverPin) == LOW) {
        TIMER_RESET;
        return;
      }

      uint16_t pulseLength = TIMER_COUNT;
      uint16_t rxBit = 0;

      if (pulseLength > HDRPULSELEN) {
        _receivedValue = 0;
        _receivedBits = 0;
        _oneBits = 0;
        return;
      }
      else if (pulseLength > ONEPULSELEN) {
        rxBit = 1;
        ++_oneBits;
      }

      _receivedValue = (_receivedValue << 1) | rxBit;
      ++_receivedBits;
      if (_receivedBits == 16) {
        if ((_oneBits & 1) == 0) {
          _ringBuffer->push(_receivedValue);
        }
      }
    }

  private:
    int _receiverPin;

    volatile uint16_t _receivedValue;
    volatile int _receivedBits;
    volatile int _oneBits;

    RingBuffer* _ringBuffer;

    #ifdef ESP32
    hw_timer_t* _timer;
    const int HDRPULSELEN = 2000;
    const int ONEPULSELEN = 100;
    #else
    const int HDRPULSELEN = 4000;
    const int ONEPULSELEN = 2000;
    #endif
};

void irIsr() {
  iamReceiver->handleInterrupt();
}
