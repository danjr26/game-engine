#include "per_frame_updateable_object.h"
#include "game_engine.h"

PerFrameUpdateableObject::PerFrameUpdateableObject() :
numberToSkip(0),
skipCount(0) {
	//GE.Per_Frame_Update().Add(this);
}

PerFrameUpdateableObject::~PerFrameUpdateableObject() {
	GE.Per_Frame_Update().Remove(this);
}

void PerFrameUpdateableObject::Next_Frame(double in_dt) {
	if (skipCount++ >= numberToSkip) {
		Update(in_dt);
	}
}

void PerFrameUpdateableObject::Skip_Frames(ubyte in_numberToSkip) {
	numberToSkip = in_numberToSkip;
}
