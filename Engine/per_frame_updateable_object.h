#ifndef PER_FRAME_UPDATEABLE_OBJECT_H
#define PER_FRAME_UPDATEABLE_OBJECT_H

#include "definitions.h"

class PerFrameUpdateableObject {
protected:
	ubyte mNumberToSkip;
	ubyte mSkipCount;
	double mMultiplier;

public:
	PerFrameUpdateableObject();
	virtual ~PerFrameUpdateableObject();
	virtual void update(double i_dt) = 0;
	virtual void updateLate(double i_dt);
	void nextFrame(double i_dt);
	void nextFrameLate(double i_dt);
	void skipFrames(ubyte i_numberToSkip);
	virtual double getMultiplier() const;
	virtual void setMultiplier(double i_multiplier);
};

#endif