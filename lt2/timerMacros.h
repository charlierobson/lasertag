#pragma once


#ifdef ESP32

#define TIMER_INIT _timer = timerBegin(3, 80, true)
#define TIMER_RESET timerWrite(_timer, 0)
#define TIMER_COUNT timerRead(_timer)
#define TIMER_INITPWM ledcSetup(0, 38000, 8); ledcAttachPin(11, 0)

#else

// set up timer 1 for a simple count
#define TIMER_INIT  TCCR1A = 0;TCCR1B = 0; TCCR1B |= (1 << CS11); TCNT1 = 0

#define TIMER_RESET TCNT1 = 0
#define TIMER_COUNT TCNT1

// set up Timer 2 for a (good enough near) 38khz rate. 0C2A pin is toggled on compare match, arduino PIN11
#define TIMER_INITPWM  TCCR2A = _BV (COM2A0) | _BV(WGM21); TCCR2B = _BV (CS20); OCR2A =  209

#endif
