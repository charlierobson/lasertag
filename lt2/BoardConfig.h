#pragma once

// wire col  signal    esp   ard
// --------  ------    ---   ---
//    black  GND
//    brown  audio     25
//      red  5v
//   orange  ws2812    5
//   yellow  IR rx     23
//    green  rbutton   19
//     blue  trigger   18
//   purple  IR tx     26

#ifdef ESP32
#define PIN_TRIGGER 18
#define PIN_RBUTTON 19
#define PIN_IR_RECIEVER 23
#define PIN_IR_TRANSMITTER 26
#define PIN_NOTIFY_LED 13
#define PIN_HARD_ON 17

#else
#define PIN_TRIGGER 4
#define PIN_IR_RECIEVER 3
#define PIN_NOTIFY_LED 5
#define PIN_IR_TRANSMITTER 11
#define PIN_LBUTTON 6
#define PIN_RBUTTON 6
#define PIN_HARD_ON 7
#endif
