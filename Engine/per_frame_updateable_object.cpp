#include "per_frame_updateable_object.h"
#include "game_engine.h"

PerFrameUpdateableObject::PerFrameUpdateableObject() :
mNumberToSkip(0),
mSkipCount(0),
mMultiplier(1.0) {
	setMultiplier(1.0);
}

PerFrameUpdateableObject::~PerFrameUpdateableObject() {
	if (GameEngine::exists()) {
		GE.perFrameUpdate().remove(this);
	}
}

void PerFrameUpdateableObject::updateLate(double i_dt) 
{}

void PerFrameUpdateableObject::nextFrame(double i_dt) {
	if (mSkipCount++ >= mNumberToSkip) {
		update(mMultiplier * i_dt);
	}
}

void PerFrameUpdateableObject::nextFrameLate(double i_dt) {
	if (mSkipCount++ >= mNumberToSkip) {
		updateLate(mMultiplier * i_dt);
	}
}

void PerFrameUpdateableObject::skipFrames(ubyte i_numberToSkip) {
	mNumberToSkip = i_numberToSkip;
}

double PerFrameUpdateableObject::getMultiplier() const {
	return mMultiplier;
}

void PerFrameUpdateableObject::setMultiplier(double i_multiplier) {
	mMultiplier = i_multiplier;
}
