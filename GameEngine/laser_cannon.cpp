#include "laser_cannon.h"
#include "laser_cannon_bullet.h"
#include "log.h"
#include "game_engine.h"

LaserCannon::LaserCannon() :
	mState(State::off),
	mAccum(0),
	mReloadTime(0.1),
	mSpreadAngle(0.05),
	mRecoil(2.0) 
{}

void LaserCannon::update(double in_dt, Feedback* out_feedback) {
	mAccum += in_dt;

	if (mState == State::on && mAccum >= mReloadTime) {
		LaserCannonBullet* bullet = new LaserCannonBullet;

		Vector2d bulletPosition;
		bulletPosition = getTransform().localToWorldPoint(bulletPosition);
		bullet->getTransform().setLocalPosition(bulletPosition);

		Rotation2d bulletRotation(GEUtil::random<double>(-mSpreadAngle, mSpreadAngle));
		bulletRotation = getTransform().localToWorldRotation(bulletRotation);
		bullet->getTransform().setLocalRotation(bulletRotation);

		mAccum = 0;

		if (out_feedback != nullptr) {
			out_feedback->mImpulse = bulletRotation.applyTo(Vector2d(-mRecoil, 0.0));
		}
	}
}

void LaserCannon::startFire() {
	mState = State::on;
}

void LaserCannon::endFire() {
	mState = State::off;
}
