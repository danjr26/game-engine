#include "per_frame_update_manager.h"
#include "game_engine.h"

PerFrameUpdateManager::PerFrameUpdateManager() 
{}

void PerFrameUpdateManager::update(double in_dt) {
	for (auto it = mUpdateables.begin(); it != mUpdateables.end(); it++) {
		if ((*it)->isEnabled()) (*it)->nextFrame(in_dt);
	}
	for (auto it = mUpdateables.begin(); it != mUpdateables.end(); it++) {
		if ((*it)->isEnabled()) (*it)->nextFrameLate(in_dt);
	}
}

void PerFrameUpdateManager::add(PerFrameUpdateableObject* in_updateable) {
	if (in_updateable == nullptr) throw InvalidArgumentException();
	mUpdateables.push_back(in_updateable);
}

void PerFrameUpdateManager::remove(PerFrameUpdateableObject* in_updateable) {
	auto position = std::find(mUpdateables.begin(), mUpdateables.end(), in_updateable);
	if (position != mUpdateables.end()) {
		mUpdateables.erase(position);
	}
}
