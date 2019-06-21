#include "Gun.h"
#include "Receiver.h"

Gun* gun;
Receiver* receiver;

enum GameState
{
  RESETTING,
  STARTING,
  RUNNING,
  OVER
};


struct GameConfig
{
  const int TEAMID = 1;
  const int PLAYERID = 1;
  const int CLIPSIZE = 20;
  const int LIVES = 3;

  int lives;

  GameState state;
} gameConfig;


void setup() {
  gun = new Gun(gameConfig.CLIPSIZE);
  gameConfig.state = RESETTING;
}


void loop() {
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
