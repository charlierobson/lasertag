
// go here to change the setup for the game/gun you're compiling for
#include "GameConfig.h"

#include "Gun.h"
#include "PlayerGun.h"
#include "RefereeGun.h"

#include "Receiver.h"

Gun* gun;
Receiver* receiver;


GameConfig gameConfig;


void setup()
{
  if (gameConfig.REFEREEGUN) {
    gun = new RefereeGun(gameConfig);
  } else {
    gun = new PlayerGun(gameConfig);
  }

  gameConfig.state = RESETTING;

  Serial.begin(115200);
}


void loop()
{
  switch (gameConfig.state) {
    case RESETTING:
      gun->begin();
      receiver->begin();

      gameConfig.lives = gameConfig.LIVES;
      gameConfig.state = STARTING;
      break;

    case STARTING:
      gameConfig.state = RUNNING;
      break;

    case RUNNING:
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
