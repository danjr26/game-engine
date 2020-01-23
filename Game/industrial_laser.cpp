#include "industrial_laser.h"

IndustrialLaser::IndustrialLaser() :
	mState(State::off),
	mWarmTime(0.5),
	mAccum(0.0) {

	mBeam.getTransform().setParent(&getTransform());
	IndustrialLaserBeam* beam = &mBeam;
	mBeam.setToggle(false);
	mBeam.setFilter(
		[beam] (Collider2d* i_mask) -> bool {
			return !beam->getIgnoreMasks().count(i_mask);
		}
	);
}

void IndustrialLaser::update(double i_dt, Feedback* o_feedback) {
	mAccum += i_dt;

	switch (mState) {
	case State::off:
		mBeam.setToggle(false);
		break;
	case State::warm:
		mBeam.setToggle(false);
		if (mAccum >= 0.75) {
			mState = State::on;
			mAccum = 0.0;
		}
		break;
	case State::on:
		mBeam.setToggle(true);
		if (mAccum >= 2.0) {
			mState = State::warm;
			mAccum = 0.0;
		}
		break;
	}
}

void IndustrialLaser::startFire() {
	if (mState == State::off) {
		mState = State::warm;
		mAccum = 0.0;
	}
}

void IndustrialLaser::endFire() {
	mState = State::off;
	mAccum = 0.0;
}

void IndustrialLaser::pushIgnoreMasks() {
	mBeam.setIgnoreMasks(mIgnoreMasks);
}
