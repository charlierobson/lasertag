# lasertag

### What
Simple laser-tag style game implementation using hacked Namco G-con and Arduino.

### Why
Why not.

### How

#### Shot detection
TSOP38238 style IR receiver is attached to external interrupt pin. On change interrupt detects a falling or rising edge. On falling edge timer 1 is reset to 0 where it counts up in counter mode. On rising edge timer 1's value is stashed in a circular buffer. Because this happens in an interrupt we never miss a hit.

The buffer is checked every game loop and if it contains enough bits for a shot then the recevied data is evaluated. The bits have the following meaning:

| 16 |  15-10 | 9-1 | 0 |
|----|--------|-----|---|
| start bit | command code | command data | parity check bit |

Parity is even. Bits are encoded similarly to 'miles tag' standard. Multiples of 600us pulses are transmitted. The length of a bit denoted its value. 4x600us for the start bit, 2x600us for a '1' bit and 1x600us for a '0' bit. Signal is RTZ, with a 600us high period separating bits.

+ = high period of 600us
- = low period of 600us

```
... ++++++++++----+--+-+--+--+-+--+--+-+-+-+-+-+-+--+-+--+--++++++ ...
                    1 0  1  1 0  1  1 0 0 0 0 0 0  1 0  1  1
```

Command code: 101101
Command data: 1000000101
Parity: 1

#### Shot transmission

Generation of 38khz signal is handed off to hardware. Timer 2 is used to toggle an output pin on hardware timer value match. The signal runs continuously and is modulated by setting the direction of the pin under timer control. When direction is OUT then the signal is outputted to the IR LED. When it's IN the LED is always off. 

---

### Useful shiz

[Timer-free tone library](https://bitbucket.org/teckel12/arduino-timer-free-tone/wiki/Home#!example)

[Soft-latching switches](https://www.youtube.com/watch?v=7D9L9oS4AJM)

[Audio using DDS](https://hackaday.com/2016/02/12/embed-with-elliot-audio-playback-with-direct-digital-synthesis/)

[38khz pwm using timer hardware](https://forum.arduino.cc/index.php?topic=102430.msg773556#msg773556)

[Lense thoughts 1](http://webcache.googleusercontent.com/search?q=cache:d14Z821idtkJ:www.lasertagparts.com/mtoptics.htm+&cd=1&hl=en&ct=clnk&gl=uk)

[Lense thoughts 2](http://webcache.googleusercontent.com/search?q=cache:e1SMQ_9_ErMJ:alumnus.caltech.edu/~leif/infratag/lens_choice.html+&cd=3&hl=en&ct=clnk&gl=uk)

[DIY gun guide](http://www.lasertag.co.uk/PDF_files/sgconst.pdf)
