#include "burning_eye_health.h"
#include "misc.h"
#include "burning_eye.h"

burning_eye::Health::Health(BurningEye& i_parent) :
	mParent(i_parent),
	mHealth(10.0)
{}

void burning_eye::Health::receiveDamage(const DamagePacket& i_damage, double i_dt) {
	switch (i_damage.mMethod) {
	case DamagePacket::Method::bulk:
		mHealth = geutil::max(mHealth - i_damage.mAmount, 0.0f);
		mParent.getRigidBody().applyLocalImpulse(i_damage.mImpulse);
		break;
	case DamagePacket::Method::per_second:
		mHealth = geutil::max(mHealth - i_damage.mAmount * (float)i_dt, 0.0f);
		LocatedVector2d impulse = i_damage.mImpulse;
		impulse.mVector *= i_dt;
		mParent.getRigidBody().applyLocalImpulse(impulse);
		break;
	}
}

bool burning_eye::Health::isDead() const {
	return mHealth <= 0.0f;
}
