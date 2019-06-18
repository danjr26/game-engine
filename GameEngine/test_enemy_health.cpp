#include "test_enemy_health.h"
#include "misc.h"
#include "test_enemy.h"

TestEnemyHealth::TestEnemyHealth(TestEnemy& in_parent) :
	mParent(in_parent),
	mHealth(10.0)
{}

void TestEnemyHealth::receiveDamage(const DamagePacket& in_damage, double in_dt) {
	switch (in_damage.mMethod) {
	case DamagePacket::Method::bulk:
		mHealth = GEUtil::max(mHealth - in_damage.mAmount, 0.0f);
		mParent.getRigidBody().applyLocalImpulse(in_damage.mImpulse);
		break;
	case DamagePacket::Method::per_second:
		mHealth = GEUtil::max(mHealth - in_damage.mAmount * (float)in_dt, 0.0f);
		LocatedVector2d impulse = in_damage.mImpulse;
		impulse.mVector *= in_dt;
		mParent.getRigidBody().applyLocalImpulse(impulse);
		break;
	}
}

bool TestEnemyHealth::isDead() const {
	return mHealth <= 0.0f;
}
