#ifndef __GAMECONFIG_H
#define __GAMECONFIG_H

// this should be all you change when compiling for different guns/games

enum GameState
{
  RESETTING,
  STARTING,
  RUNNING,
  OVER
};

typedef struct
{
  const bool REFEREE_GUN = true;      // a referee gun has the power to affect players: reset, heal, admin kill etc 

  const int TEAMID = 1;               // data transmitted by gun to identify player.
  const int PLAYERID = 1;  

  const int CLIPSIZE = 20;            // gun's clip size, shots available to it per game
  const int LIVES = 3;                // lives per game

  int lives;

  GameState state;
}
GameConfig;

#endif
