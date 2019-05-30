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

void Minigun::update(double in_dt, Feedback* out_feedback) {
	mAccum += in_dt;

	if (mState == State::on && mAccum >= mReloadTime) {
		MinigunBullet* bullet = new MinigunBullet;

		Vector2d bulletPosition;
		bulletPosition = getTransform().localToWorldPoint(bulletPosition);
		bullet->getTransform().setLocalPosition(bulletPosition);

		Rotation2d bulletRotation(GEUtil::random<double>(-mSpreadAngle, mSpreadAngle));
		bulletRotation = getTransform().localToWorldRotation(bulletRotation);
		bullet->getTransform().setLocalRotation(bulletRotation);

		mAccum = 0.0;

		if (out_feedback != nullptr) {
			out_feedback->mImpulse = bulletRotation.applyTo(Vector2d(-mRecoil, 0.0));
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
