#pragma once


#ifdef ESP32

#define PWM_INIT ledcSetup(0, 38000, 8); ledcAttachPin(_irLEDPin, 0)
#define PWM_ENABLE ledcWrite(0, 128)
#define PWM_DISABLE ledcWrite(0, 0)

#else

// set up Timer 2 for a (good enough near) 38khz rate. 0C2A pin is toggled on compare match,
// arduino PIN11
#define PWM_INIT TCCR2A = _BV (COM2A0) | _BV(WGM21); TCCR2B = _BV (CS20); OCR2A =  209
#define PWM_ENABLE pinMode(11, OUTPUT)
#define PWM_DISABLE pinMode(11, INPUT)

#endif
