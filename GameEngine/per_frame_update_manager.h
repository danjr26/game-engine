#ifndef PER_FRAME_UPDATE_MANAGER_H
#define PER_FRAME_UPDATE_MANAGER_H

#include "stepper.h"
#include "per_frame_updateable_object.h"
#include <vector>

class PerFrameUpdateManager {
private:
	std::vector<PerFrameUpdateableObject*> mUpdateables;

public:
	PerFrameUpdateManager();
	void Update(double in_dt);
	void Add(PerFrameUpdateableObject* in_updateable);
	void Remove(PerFrameUpdateableObject* in_updateable);
};

#endif