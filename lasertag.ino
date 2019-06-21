#include <avr/io.h>
#include <avr/interrupt.h>

#define LEDPIN 13

#define TRANSMITPIN 2
#define DETECTORPIN 3


extern void initialiseReceiver();
extern bool playerWasShot();
extern bool triggerPulled();
extern void shoot();


void setup()
{
  Serial.begin(9600);

  pinMode(LEDPIN, OUTPUT);
  pinMode(TRANSMITPIN, OUTPUT);
  pinMode(TRIGGERPIN, INPUT_PULLUP);
  pinMode(DETECTORPIN, INPUT_PULLUP);

  initialiseReceiver();
}


void loop()
{
  if (playerWasShot()) {
    Serial.println("ow!");
  }

  if (triggerPulled()) {
    Serial.println("pow!");
    shoot();
  }
}
