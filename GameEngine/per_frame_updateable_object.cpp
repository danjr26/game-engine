#include "per_frame_updateable_object.h"
#include "game_engine.h"

PerFrameUpdateableObject::PerFrameUpdateableObject() :
mNumberToSkip(0),
mSkipCount(0) 
{}

PerFrameUpdateableObject::~PerFrameUpdateableObject() {
	GE.perFrameUpdate().remove(this);
}

void PerFrameUpdateableObject::nextFrame(double in_dt) {
	if (mSkipCount++ >= mNumberToSkip) {
		update(in_dt);
	}
}

void PerFrameUpdateableObject::skipFrames(ubyte in_numberToSkip) {
	mNumberToSkip = in_numberToSkip;
}
