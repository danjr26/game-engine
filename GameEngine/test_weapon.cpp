#include "test_weapon.h"
#include "test_bullet.h"
#include "log.h"

TestWeapon::TestWeapon() :
	mIsFiring(false),
	mAccum(0),
	mReloadTime(0.5)
{}

void TestWeapon::update(double in_dt) {
	mAccum += in_dt;
	if (mIsFiring && mAccum >= mReloadTime) {
		Log::main("fire");
		mAccum = 0;
	}
}

void TestWeapon::startFire() {
	mIsFiring = true;
}

void TestWeapon::endFire() {
	mIsFiring = false;
}
