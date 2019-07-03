#ifndef TEST_ENEMY_HEALTH_H
#define TEST_ENEMY_HEALTH_H

#include "damage_receiver.h"

class BurningEye;

class BurningEyeHealth : public DamageReceiver {
private:
	BurningEye& mParent;
	float mHealth;

public:
	BurningEyeHealth(BurningEye& in_parent);
	void receiveDamage(const DamagePacket& in_damage, double in_dt) override;
	bool isDead() const;
};

#endif