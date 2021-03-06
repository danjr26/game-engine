#include "chai_bolt.h"

ChainBolt::ChainBolt() :
	mState(State::off),
	mAccum(0.0),
	mReloadTime(0.8),
	mRecoil(3.0)
{}

void ChainBolt::update(double i_dt, Feedback* o_feedback) {
	mAccum += i_dt;

	if (mState == State::on && mAccum >= mReloadTime) {
		
		mAccum = 0.0;
	}
}

void ChainBolt::startFire() {
	if (mState != State::on) {
		mState = State::on;
		mAccum = 0.0;
	}
}

void ChainBolt::endFire() {
	if (mState != State::off) {
		mState = State::off;
		mAccum = 0.0;
	}
}
