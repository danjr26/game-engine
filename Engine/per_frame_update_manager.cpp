#include "per_frame_update_manager.h"
#include "game_engine.h"

PerFrameUpdateManager::PerFrameUpdateManager() 
{}

void PerFrameUpdateManager::update(double i_dt) {
	for (auto it = mUpdateables.begin(); it != mUpdateables.end(); it++) {
		if ((*it)->isEnabled()) (*it)->nextFrame(i_dt);
	}
	for (auto it = mUpdateables.begin(); it != mUpdateables.end(); it++) {
		if ((*it)->isEnabled()) (*it)->nextFrameLate(i_dt);
	}
}

void PerFrameUpdateManager::add(PerFrameUpdateableObject* i_updateable) {
	if (i_updateable == nullptr) throw InvalidArgumentException();
	mUpdateables.push_back(i_updateable);
}

void PerFrameUpdateManager::remove(PerFrameUpdateableObject* i_updateable) {
	auto position = std::find(mUpdateables.begin(), mUpdateables.end(), i_updateable);
	if (position != mUpdateables.end()) {
		mUpdateables.erase(position);
	}
}
