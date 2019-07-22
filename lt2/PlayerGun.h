#pragma once

class PlayerGun : public Gun
{
public:
  PlayerGun(GameConfig& gameConfig, int triggerPin) : Gun(gameConfig, triggerPin) {
  }

  void update() {
    if (!_ammoRemaining) return;
    if (!triggerPulled()) return;
    fire(_shotBits);
  }
};
