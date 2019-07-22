#pragma once

class RingBuffer
{
  public:
    static const int RBSIZE = 64;

    RingBuffer() :
      _head(0),
      _tail(0)
    {
    }

    bool dataPresent() {
      return _head != _tail;
    }

    int dataCount() {
      int entries = _head - _tail;
      if (entries < 0) entries += RBSIZE;
      return entries;
    }

    uint16_t pop() {
      uint16_t val = _ringBuffer[_tail];
      _tail = (_tail + 1) & (RBSIZE - 1);
      return val;
    }

    uint16_t push(uint16_t val) {
      _ringBuffer[_head] = val;
      _head = (_head + 1) & (RBSIZE - 1);
    }

  private:
    volatile int _head;
    volatile int _tail;
    uint16_t _ringBuffer[RBSIZE];
};
