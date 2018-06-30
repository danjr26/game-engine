#include "per_frame_updateable_object.h"
#include "game_engine.h"

PerFrameUpdateableObject::PerFrameUpdateableObject() {
	GE.Per_Frame_Update().Add(this);
}

PerFrameUpdateableObject::~PerFrameUpdateableObject() {
	GE.Per_Frame_Update().Remove(this);
}
