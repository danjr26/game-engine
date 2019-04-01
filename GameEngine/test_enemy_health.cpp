#include "test_enemy_health.h"
#include "misc.h"
#include "test_enemy.h"

TestEnemyHealth::TestEnemyHealth(TestEnemy& in_parent) :
	mParent(in_parent),
	mHealth(10.0)
{}

void TestEnemyHealth::damage(const DamagePacket& in_damage) {
	mHealth = GEUtil::max(mHealth - in_damage.mAmount, 0.0f);
}

bool TestEnemyHealth::isDead() const {
	return mHealth <= 0.0f;
}
