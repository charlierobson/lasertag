
class PlayerGun : public Gun
{
public:
  PlayerGun(GameConfig& gameConfig) : Gun(gameConfig) {
  }

  void update() {
    if (!_ammoRemaining) return;
    if (!triggerPulled()) return;
    fire(shotBits);
  }
};
