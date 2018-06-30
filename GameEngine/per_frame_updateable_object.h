#ifndef PER_FRAME_UPDATEABLE_OBJECT_H
#define PER_FRAME_UPDATEABLE_OBJECT_H

#include "disableable_object.h"

class PerFrameUpdateableObject : public DisableableObject {
public:
	PerFrameUpdateableObject();
	virtual ~PerFrameUpdateableObject();
	virtual void Update(double in_dt) = 0;
};

#endif