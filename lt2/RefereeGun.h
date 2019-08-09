#pragma once

class RefereeGun : public Gun
{
public:
  RefereeGun(GameConfig& gameConfig, int irLEDPin) : Gun(gameConfig, irLEDPin) {
  }

  void update() {
    fire(_shotBits);
  }
};
