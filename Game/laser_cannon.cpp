#include "laser_cannon.h"
#include "laser_cannon_bullet.h"
#include "log.h"
#include "game_engine.h"

LaserCannon::LaserCannon() :
	mState(State::off),
	mAccum(0),
	mReloadTime(0.2),
	mSpreadAngle(0.05),
	mRecoil(2.0) 
{}

void LaserCannon::update(double i_dt, Feedback* o_feedback) {
	mAccum += i_dt;

	if (mState == State::on && mAccum >= mReloadTime) {
		LaserCannonBullet* bullet = new LaserCannonBullet;

		Vector2d bulletPosition;
		bulletPosition = getTransform().localToWorldPoint(bulletPosition);
		bullet->getTransform().setLocalPosition(bulletPosition);

		Rotation2d bulletRotation(geutil::random<double>(-mSpreadAngle, mSpreadAngle));
		bulletRotation = getTransform().localToWorldRotation(bulletRotation);
		bullet->getTransform().setLocalRotation(bulletRotation);

		mAccum = 0;

		if (o_feedback != nullptr) {
			o_feedback->mImpulse = bulletRotation.applyTo(Vector2d(-mRecoil, 0.0));
		}
	}
}

void LaserCannon::startFire() {
	mState = State::on;
}

void LaserCannon::endFire() {
	mState = State::off;
}
