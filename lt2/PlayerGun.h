#pragma once

class PlayerGun : public Gun
{
public:
  PlayerGun(GameConfig& gameConfig, int irLEDPin) : Gun(gameConfig, irLEDPin) {
  }

  void update() {
    fire(_shotBits);
  }
};
