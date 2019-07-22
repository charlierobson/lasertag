#pragma once

class RefereeGun : public Gun
{
public:
  RefereeGun(GameConfig& gameConfig, int triggerPin) : Gun(gameConfig, triggerPin) {
  }

  void update() {
    if (!_ammoRemaining) return;
    if (!triggerPulled()) return;
    fire(_shotBits);
  }
};
