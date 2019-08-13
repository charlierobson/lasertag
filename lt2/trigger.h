#pragma once

class Trigger
{
private:
    int _state;
    int _pinNum;
    uint16_t _counts;
    uint16_t _shiftreg;

public:
    Trigger(int pinNum) : 
    _pinNum(pinNum),
    _shiftreg(0xffff),
    _counts(0),
    _state(0) {
        pinMode(_pinNum, INPUT_PULLUP);
    }

    void update() {
        _shiftreg <<= 1;
        _shiftreg |= digitalRead(_pinNum) ? 1 : 0;

        if (_shiftreg == 0xffff) _state = 0; 
        else if (_shiftreg == 0x8000) _state = 1;
        else if (_shiftreg == 0x0000) _state = 2;
        else if (_shiftreg == 0x0001) _state = 3;

        if (_state == 2) {
            if (_counts != 0xffff) {
                ++_counts;
            }
        }
        else _counts = 0;
    }

    int state() {
        return _state;
    }

    int timeHeld() {
        return _counts;
    }
};
