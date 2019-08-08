#pragma once

class RefereeGun : public Gun
{
public:
  RefereeGun(GameConfig& gameConfig, Trigger* trigger) : Gun(gameConfig, trigger) {
  }

  void update() {
    if (!_ammoRemaining) return;

    if (_trigger->state() != 1) return;

    fire(_shotBits);
  }
};
