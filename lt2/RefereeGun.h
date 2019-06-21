
class RefereeGun : public Gun
{
public:
  RefereeGun(GameConfig& gameConfig) : Gun(gameConfig) {
  }

  void update() {
    if (!_ammoRemaining) return;
    if (!triggerPulled()) return;
    fire(shotBits);
  }
};
