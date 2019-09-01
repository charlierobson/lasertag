
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
  Serial.begin(115200);
  delay(1000);
  Serial.println("LASERTAG");

  gunTrigger = new Trigger(PIN_TRIGGER);
  reloadTrigger = new Trigger(PIN_RBUTTON);

  // turn off the soft power switch and loop until the trigger is pulled
  pinMode(PIN_HARD_ON, OUTPUT);
  digitalWrite(PIN_HARD_ON, LOW);

  Serial.println("WAIT TRIGGER ON");

  while(gunTrigger->state() == Trigger::NOTTRIGGERED) {
    delay(1);
    gunTrigger->update();
  }

  // hold the power on
  digitalWrite(PIN_HARD_ON, HIGH);

  sfx = new SFX();
  sfx->begin();

  // play the welcome sound then flush the audio buffer to ensure there's no glitch
  sfx->playSoundSync(SFX_HELLO);
  sfx->flush();

  Serial.println("WAIT TRIGGER OFF");

  while(gunTrigger->state() == Trigger::HELD) {
    delay(1);
    gunTrigger->update();
  }

  Serial.println("OK");

  gun = new PlayerGun(PIN_IR_TRANSMITTER, gameConfig.TEAMID, gameConfig.PLAYERID);

  receiver = new Receiver(PIN_IR_RECIEVER);

  gameConfig.state = RESETTING;

  pinMode(PIN_NOTIFY_LED, OUTPUT);
  digitalWrite(PIN_NOTIFY_LED, LOW);
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
      if (gunTrigger->state() == Trigger::JUSTTRIGGERED) {
        if (gameConfig.shotsRemaining != 0) {
          sfx->playSound(SFX_SHOT);
          gun->transmitShot(0);
          --gameConfig.shotsRemaining;
        } else {
          sfx->playSound(SFX_EMPTY);
        }
      }

      if (reloadTrigger->state() == Trigger::JUSTTRIGGERED) {
        if (gameConfig.clipsRemaining != 0) {
          sfx->playSound(SFX_RELOAD);
          gameConfig.shotsRemaining = gameConfig.CLIPSIZE;
          -- gameConfig.clipsRemaining;
        }
        else {
          sfx->playSound(SFX_NOPE);
        }
      }

      if (reloadTrigger->timeHeld() > 400) {
        // 2000ms/5
        sfx->playSoundSync(SFX_SHUTTINGDOWN);
        digitalWrite(PIN_HARD_ON, LOW);
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
