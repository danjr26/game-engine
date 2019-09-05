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

void PerFrameUpdateableObject::updateLate(double in_dt) 
{}

void PerFrameUpdateableObject::nextFrame(double in_dt) {
	if (mSkipCount++ >= mNumberToSkip) {
		update(mMultiplier * in_dt);
	}
}

void PerFrameUpdateableObject::nextFrameLate(double in_dt) {
	if (mSkipCount++ >= mNumberToSkip) {
		updateLate(mMultiplier * in_dt);
	}
}

void PerFrameUpdateableObject::skipFrames(ubyte in_numberToSkip) {
	mNumberToSkip = in_numberToSkip;
}

double PerFrameUpdateableObject::getMultiplier() const {
	return mMultiplier;
}

void PerFrameUpdateableObject::setMultiplier(double in_multiplier) {
	mMultiplier = in_multiplier;
}
