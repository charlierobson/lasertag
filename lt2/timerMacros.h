#pragma once


#ifdef ESP32

#define TIMER_INIT _timer = timerBegin(3, 80, true)
#define TIMER_RESET timerWrite(_timer, 0)
#define TIMER_COUNT timerRead(_timer)

#else

#define TIMER_INIT  TCCR1A = 0;TCCR1B = 0; TCCR1B |= (1 << CS11); TCNT1 = 0
#define TIMER_RESET TCNT1 = 0
#define TIMER_COUNT TCNT1

#endif
