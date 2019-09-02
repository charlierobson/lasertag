#pragma once

class Trigger
{
private:
    int _state;
    int _pinNum;
    uint16_t _counts;
    uint16_t _shiftreg;

public:
    static enum {
        NOTTRIGGERED,
        JUSTTRIGGERED,
        HELD,
        JUSTRELEASED,
    } tstate;

    Trigger(int pinNum) : 
    _pinNum(pinNum),
    _shiftreg(0xffff),
    _counts(0),
    _state(NOTTRIGGERED) {
        pinMode(_pinNum, INPUT_PULLUP);
    }

    void update() {
        int triggered = digitalRead(_pinNum) ? 1 : 0;

        _shiftreg <<= 1;
        _shiftreg |= triggered;

        switch(_state) {
            case NOTTRIGGERED:
                if (triggered == 0) _state = JUSTTRIGGERED;
                break;
            case JUSTTRIGGERED:
                _state = HELD;
                break;
            case HELD:
                if ((_shiftreg & 0xff) == 0xff) _state = NOTTRIGGERED;
                break;
        }

        // _state = NOTTRIGGERED;
        // int b4 = _shiftreg & 0xff;
        // if (b4 == 0xfe) _state = JUSTTRIGGERED;
        // else if (b4 == 0x00) _state = HELD;
        // else if (b4 == 0x01) _state = JUSTRELEASED;

        if (_state == HELD) {
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
