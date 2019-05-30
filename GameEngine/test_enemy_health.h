#ifndef TEST_ENEMY_HEALTH_H
#define TEST_ENEMY_HEALTH_H

#include "damage_receiver.h"

class TestEnemy;

class TestEnemyHealth : public DamageReceiver {
private:
	TestEnemy& mParent;
	float mHealth;

public:
	TestEnemyHealth(TestEnemy& in_parent);
	void receiveDamage(const DamagePacket& in_damage, double in_dt) override;
	bool isDead() const;
};

#endif