#include "test_weapon.h"
#include "test_bullet.h"
#include "log.h"

TestWeapon::TestWeapon() :
	mIsFiring(false),
	mAccum(0),
	mReloadTime(0.2)
{}

void TestWeapon::update(double in_dt) {
	mAccum += in_dt;
	if (mIsFiring && mAccum >= mReloadTime) {
		TestBullet* bullet = new TestBullet;
		bullet->getTransform().setLocalPosition(getTransform().localToWorldPoint(Vector2d()));
		bullet->getTransform().setLocalRotation(getTransform().localToWorldRotation(Rotation2d()));
		mAccum = 0;
	}
}

void TestWeapon::startFire() {
	mIsFiring = true;
}

void TestWeapon::endFire() {
	mIsFiring = false;
}
