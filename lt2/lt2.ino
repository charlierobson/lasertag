
// go here to change the setup for the game/gun you're compiling for
#include "GameConfig.h"
#include "BoardConfig.h"

#include "Audio.h"
#include "Trigger.h"
#include "Gun.h"
#include "PlayerGun.h"
#include "RefereeGun.h"
#include "Receiver.h"

Gun* gun;
Receiver* receiver;
Trigger* gunTrigger;
Trigger* reloadTrigger;
SFX* sfx;

GameConfig gameConfig;

unsigned long ledOffTime = 0;

void indicatorOn(unsigned int duration) {
  digitalWrite(PIN_NOTIFY_LED, HIGH);
  ledOffTime = millis() + duration;
}


void setup()
{
  gunTrigger = new Trigger(PIN_TRIGGER);
  reloadTrigger = new Trigger(PIN_RBUTTON);
  
  gun = new PlayerGun(PIN_IR_TRANSMITTER, gameConfig.TEAMID, gameConfig.PLAYERID);

  receiver = new Receiver(PIN_IR_RECIEVER);

  gameConfig.state = RESETTING;

  sfx = new SFX();
  sfx->begin();

  pinMode(PIN_NOTIFY_LED, OUTPUT);
  digitalWrite(PIN_NOTIFY_LED, LOW);

  Serial.begin(115200);
  delay(1000);
  Serial.println("LASERTAG");
}


void loop()
{
  static unsigned long nextUpdateTime = 1;

  while (millis() < nextUpdateTime) {
    sfx->update();
  }
  nextUpdateTime += 5;

  gunTrigger->update();
  reloadTrigger->update();

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

      gameConfig.shotsRemaining = gameConfig.CLIPSIZE;
      gameConfig.clipsRemaining = gameConfig.CLIPCOUNT;
      break;

    case STARTING:
      gameConfig.state = RUNNING;
      break;

    case RUNNING:
      if (gunTrigger->state() == 2) {
        if (gameConfig.shotsRemaining != 0) {
          sfx->playSound(SFX_SHOT);
          gun->transmitShot(0);
          --gameConfig.shotsRemaining;
        } else {
          sfx->playSound(SFX_EMPTY);
        }
      }

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
