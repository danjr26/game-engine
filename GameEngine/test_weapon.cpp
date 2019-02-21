#include "test_weapon.h"
#include "test_bullet.h"
#include "log.h"
#include "game_engine.h"

TestWeapon::TestWeapon() :
	mIsFiring(false),
	mAccum(0),
	mReloadTime(0.1)
{}

void TestWeapon::update(double in_dt, Feedback* out_feedback) {
	mAccum += in_dt;

	if (out_feedback != nullptr) {
		out_feedback->impulse = Vector2d();
	}

	if (mIsFiring && mAccum >= mReloadTime) {
		TestBullet* bullet = new TestBullet;

		Vector2d bulletPosition;
		bulletPosition = getTransform().localToWorldPoint(bulletPosition);
		bullet->getTransform().setLocalPosition(bulletPosition);

		Rotation2d bulletRotation(GEUtil::random<double>(-0.1, 0.1));
		bulletRotation = getTransform().localToWorldRotation(bulletRotation);
		bullet->getTransform().setLocalRotation(bulletRotation);

		mAccum = 0;

		if (out_feedback != nullptr) {
			out_feedback->impulse = bulletRotation.applyTo(Vector2d(-1.0, 0.0));
		}
	}
}

void TestWeapon::startFire() {
	mIsFiring = true;
}

void TestWeapon::endFire() {
	mIsFiring = false;
}
