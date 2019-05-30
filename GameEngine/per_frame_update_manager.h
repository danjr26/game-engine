#ifndef PER_FRAME_UPDATE_MANAGER_H
#define PER_FRAME_UPDATE_MANAGER_H

#include "stepper.h"
#include "per_frame_updateable_object.h"
#include <list>

class PerFrameUpdateManager {
private:
	std::list<PerFrameUpdateableObject*> mUpdateables;

public:
	PerFrameUpdateManager();
	void update(double in_dt);
	void add(PerFrameUpdateableObject* in_updateable);
	void remove(PerFrameUpdateableObject* in_updateable);
};

#endif