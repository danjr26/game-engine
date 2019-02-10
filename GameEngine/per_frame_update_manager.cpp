#include "per_frame_update_manager.h"
#include "game_engine.h"

PerFrameUpdateManager::PerFrameUpdateManager() 
{}

void PerFrameUpdateManager::Update(double in_dt) {
	for (uint i = 0; i < mUpdateables.size(); i++) {
		if (mUpdateables[i]->Is_Enabled()) {
			mUpdateables[i]->Next_Frame(in_dt);
		}
	}
}

void PerFrameUpdateManager::Add(PerFrameUpdateableObject* in_updateable) {
	mUpdateables.push_back(in_updateable);
}

void PerFrameUpdateManager::Remove(PerFrameUpdateableObject* in_updateable) {
	auto position = std::find(mUpdateables.begin(), mUpdateables.end(), in_updateable);
	if (position != mUpdateables.end()) {
		mUpdateables.erase(position);
	}
}
