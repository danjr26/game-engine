#include "industrial_laser.h"

IndustrialLaser::IndustrialLaser() :
	mState(State::off),
	mWarmUp(0.5),
	mAccum(0.0) {

	addIgnoreMask(&mBeam.getCollisionMask());
	mBeam.getTransform().setParent(&getTransform());
	IndustrialLaserBeam* beam = &mBeam;
	mBeam.setFilter(
		[beam] (CollisionMask2d* in_mask) -> bool {
			return !beam->getIgnoreMasks().count(in_mask);
		}
	);
}

void IndustrialLaser::update(double in_dt, Feedback* out_feedback) {
	mAccum += in_dt;

	if (mState == State::on) {

	}
}

void IndustrialLaser::startFire() {
	if (mState == State::off) {
		mState = State::warm;
	}
}

void IndustrialLaser::endFire() {
	mState = State::off;
}

void IndustrialLaser::pushIgnoreMasks() {
	mBeam.setIgnoreMasks(mIgnoreMasks);
}
