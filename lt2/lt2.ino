
// go here to change the setup for the game/gun you're compiling for
#include "GameConfig.h"

#include "Gun.h"
#include "PlayerGun.h"
#include "RefereeGun.h"

#include "Receiver.h"

Gun* gun;
Receiver* receiver;

GameConfig gameConfig;

#define PIN_TRIGGER 4
#define PIN_IR_RECIEVER 3
#define PIN_NOTIFY_LED 5

unsigned long ledOffTime = 0;

void indicatorOn(unsigned int duration) {
  digitalWrite(PIN_NOTIFY_LED, HIGH);
  ledOffTime = millis() + duration;
}


void setup()
{
  if (gameConfig.REFEREEGUN) {
    gun = new RefereeGun(gameConfig, PIN_TRIGGER);
  } else {
    gun = new PlayerGun(gameConfig, PIN_TRIGGER);
  }

  receiver = new Receiver(PIN_IR_RECIEVER);

  gameConfig.state = RESETTING;

  pinMode(PIN_NOTIFY_LED, OUTPUT);
  digitalWrite(PIN_NOTIFY_LED, LOW);

  Serial.begin(115200);
  delay(1000);
  Serial.println("LASERTAG");
}


void loop()
{
  static unsigned long lastUpdateTime = 0;

  if (ledOffTime != 0 && millis() > ledOffTime) {
    digitalWrite(PIN_NOTIFY_LED, LOW);
    ledOffTime = 0;
  }

  switch (gameConfig.state) {
    case RESETTING:
      gun->begin();
      receiver->begin();

      gameConfig.lives = gameConfig.LIVESPERGAME;
      gameConfig.state = STARTING;
      break;

    case STARTING:
      gameConfig.state = RUNNING;
      break;

    case RUNNING:
      // peg update rate to once every 2 ms
      while (millis() >> 1 != lastUpdateTime) {}
      lastUpdateTime = millis() >> 1;

      gun->update();
      receiver->update();

      if (receiver->detectedHit()) {
        --gameConfig.lives;
        if (!gameConfig.lives) {
          gameConfig.state = OVER;
        }
      }
      break;

    case OVER:
      delay(2000);
      gameConfig.state = RESETTING;
      break;
  }
}
