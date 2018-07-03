#ifndef PER_FRAME_UPDATEABLE_OBJECT_H
#define PER_FRAME_UPDATEABLE_OBJECT_H

#include "disableable_object.h"
#include "definitions.h"

class PerFrameUpdateableObject : public DisableableObject {
protected:
	ubyte numberToSkip;
	ubyte skipCount;

protected:
	void Skip_Frames(ubyte in_numberToSkip);

public:
	PerFrameUpdateableObject();
	virtual ~PerFrameUpdateableObject();
	virtual void Update(double in_dt) = 0;
};

#endif