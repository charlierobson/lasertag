#pragma once

class PlayerGun : public Gun
{
public:
  PlayerGun(GameConfig& gameConfig, Trigger* trigger, int irLEDPin) : Gun(gameConfig, trigger, irLEDPin) {
  }

  void update() {
    if (!_ammoRemaining) return;

    if (_trigger->state() != 1) return;

    fire(_shotBits);
  }
};
