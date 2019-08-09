#pragma once

class PlayerGun : public Gun
{
public:
  PlayerGun(int irLEDPin, int team, int player) : Gun(irLEDPin, team, player) {
  }

  void transmitShot(int shotType) {
    fire(_shotBits);
  }
};
