#pragma once

// wire col  signal    esp   ard
// --------  ------    ---   ---
//    black  GND
//    brown  audio     25
//      red  5v
//   green  ws2812     19
//   purple  IR rx     26
//    white  rbutton   16
//     grey  trigger   17
//   orange  IR tx     5 
//     blue  powerOn   18

#ifdef ESP32
#define PIN_TRIGGER 17
#define PIN_RBUTTON 16
#define PIN_IR_RECIEVER 26
#define PIN_IR_TRANSMITTER 5
#define PIN_NOTIFY_LED 19
#define PIN_POWER 18
#else
#define PIN_TRIGGER 4
#define PIN_IR_RECIEVER 3
#define PIN_NOTIFY_LED 5
#define PIN_IR_TRANSMITTER 11
#define PIN_LBUTTON 6
#define PIN_RBUTTON 6
#endif
