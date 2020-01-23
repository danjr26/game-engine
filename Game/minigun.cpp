#include "minigun.h"
#include "minigun_bullet.h"
#include "log.h"
#include "game_engine.h"

Minigun::Minigun() :
	mState(State::off),
	mAccum(0.05),
	mReloadTime(0.05),
	mWarmTime(1.5),
	mSpreadAngle(0.2),
	mRecoil(1.0) 
{}

void Minigun::update(double i_dt, Feedback* o_feedback) {
	mAccum += i_dt;

	if (mState == State::on && mAccum >= mReloadTime) {
		MinigunBullet* bullet = new MinigunBullet;

		Vector2d bulletPosition;
		bulletPosition = getTransform().localToWorldPoint(bulletPosition);
		bullet->getTransform().setLocalPosition(bulletPosition);

		Rotation2d bulletRotation(geutil::random<double>(-mSpreadAngle, mSpreadAngle));
		bulletRotation = getTransform().localToWorldRotation(bulletRotation);
		bullet->getTransform().setLocalRotation(bulletRotation);

		mAccum = 0.0;

		if (o_feedback != nullptr) {
			o_feedback->mImpulse = bulletRotation.applyTo(Vector2d(-mRecoil, 0.0));
		}
	}
	else if (mState == State::warm && mAccum >= mWarmTime) {
		mState = State::on;
		mAccum = mReloadTime;
	}
}

void Minigun::startFire() {
	mState = State::warm;
	mAccum = 0.0;
}

void Minigun::endFire() {
	mState = State::off;
	mAccum = 0.0;
}
