#include "burning_eye_health.h"
#include "misc.h"
#include "burning_eye.h"

burning_eye::Health::Health(BurningEye& in_parent) :
	mParent(in_parent),
	mHealth(10.0)
{}

void burning_eye::Health::receiveDamage(const DamagePacket& in_damage, double in_dt) {
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

bool burning_eye::Health::isDead() const {
	return mHealth <= 0.0f;
}
