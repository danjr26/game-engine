#ifndef TEST_ENEMY_HEALTH_H
#define TEST_ENEMY_HEALTH_H

#include "damage_packet.h"

class TestEnemy;

class TestEnemyHealth {
private:
	TestEnemy& mParent;
	float mHealth;

public:
	TestEnemyHealth(TestEnemy& in_parent);
	void damage(const DamagePacket& in_damage);
	bool isDead() const;
};

#endif