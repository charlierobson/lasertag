#pragma once

// wire col  signal    esp   ard
// --------  ------    ---   ---
//    brown  audio     25
//    white  reload    16
//   orange  IR tx     5
//      red  5v
//    black  GND
//  (yellow  unused)
//     blue  power on  18
//    green  ws2812    19
//   purple  IR rx     26
//     grey  trigger   17

#ifdef ESP32
#define PIN_TRIGGER 17
#define PIN_RBUTTON 16
#define PIN_IR_RECIEVER 26
#define PIN_IR_TRANSMITTER 5
#define PIN_NOTIFY_LED 19
#define PIN_HARD_ON 18
#else
#define PIN_TRIGGER 4
#define PIN_IR_RECIEVER 3
#define PIN_NOTIFY_LED 5
#define PIN_IR_TRANSMITTER 11
#define PIN_LBUTTON 6
#define PIN_RBUTTON 6
#define PIN_HARD_ON 7
#endif
