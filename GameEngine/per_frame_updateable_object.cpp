#include "per_frame_updateable_object.h"
#include "game_engine.h"

PerFrameUpdateableObject::PerFrameUpdateableObject() :
mNumberToSkip(0),
mSkipCount(0) 
{}

PerFrameUpdateableObject::~PerFrameUpdateableObject() {
	GE.Per_Frame_Update().Remove(this);
}

void PerFrameUpdateableObject::Next_Frame(double in_dt) {
	if (mSkipCount++ >= mNumberToSkip) {
		Update(in_dt);
	}
}

void PerFrameUpdateableObject::Skip_Frames(ubyte in_numberToSkip) {
	mNumberToSkip = in_numberToSkip;
}
