#ifndef PER_FRAME_UPDATEABLE_OBJECT_H
#define PER_FRAME_UPDATEABLE_OBJECT_H

#include "disableable_object.h"
#include "definitions.h"

class PerFrameUpdateableObject : public DisableableObject {
protected:
	ubyte mNumberToSkip;
	ubyte mSkipCount;
	double mMultiplier;

public:
	PerFrameUpdateableObject();
	virtual ~PerFrameUpdateableObject();
	virtual void update(double in_dt) = 0;
	virtual void updateLate(double in_dt);
	void nextFrame(double in_dt);
	void nextFrameLate(double in_dt);
	void skipFrames(ubyte in_numberToSkip);
	virtual double getMultiplier() const;
	virtual void setMultiplier(double in_multiplier);
};

#endif