#pragma once

// this file should be all you need to change when compiling for different guns/games

enum GameState
{
  RESETTING,
  STARTING,
  RUNNING,
  OVER
};

enum Command
{
  // non-referee player
  SHOT = 1,

  // referee commands
  RESET,
  REVIVE,
  ADMINKILL,
  GIVEAMMO
};


typedef struct
{
  const bool REFEREEGUN = true;      // a referee gun has the power to affect players: reset, heal, admin kill etc 

  const int TEAMID = 1;               // data transmitted by gun to identify player.
  const int PLAYERID = 1;  

  const int LIVESPERGAME = 3;

  int lives;

  const int CLIPSIZE = 20;            // gun's clip size, shots available to it per game
  const int CLIPCOUNT = 4;            // number of clips/reloads allowed

  int shotsRemaining;
  int clipsRemaining;

  GameState state;
}
GameConfig;
