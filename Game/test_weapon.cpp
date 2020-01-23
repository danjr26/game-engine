#include "test_weapon.h"
#include "test_bullet.h"
#include "log.h"
#include "game_engine.h"

TestWeapon::TestWeapon() :
	mIsFiring(false),
	mAccum(0),
	mReloadTime(0.1),
	mSpreadAngle(0.05),
	mRecoil(2.0)
{}

void TestWeapon::update(double i_dt, Feedback* o_feedback) {
	mAccum += i_dt;

	if (mIsFiring && mAccum >= mReloadTime) {
		TestBullet* bullet = new TestBullet;

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

void TestWeapon::startFire() {
	mIsFiring = true;
}

void TestWeapon::endFire() {
	mIsFiring = false;
}
