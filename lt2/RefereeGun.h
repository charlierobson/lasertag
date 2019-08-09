#pragma once

class RefereeGun : public Gun
{
public:
  RefereeGun(int irLEDPin, int team, int player) : Gun(irLEDPin, team, player) {
  }

  void transmitShot(int shotType) {
    fire(_shotBits);
  }
};
