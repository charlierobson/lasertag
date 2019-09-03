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
        int pinState = digitalRead(_pinNum);

        _shiftreg <<= 1;
        _shiftreg |= pinState;

        switch(_state) {
            case NOTTRIGGERED:
                if (pinState == LOW) _state = JUSTTRIGGERED;
                break;
            case JUSTTRIGGERED:
                _state = HELD;
                break;
            case HELD:
                if (_shiftreg == 0xffff) _state = JUSTRELEASED;
                break;
            case JUSTRELEASED:
                _state = NOTTRIGGERED;
                break;
        }

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
