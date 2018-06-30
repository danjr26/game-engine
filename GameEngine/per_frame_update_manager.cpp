#include "per_frame_update_manager.h"
#include "game_engine.h"

PerFrameUpdateManager::PerFrameUpdateManager() 
{}

void PerFrameUpdateManager::Update(double in_dt) {
	for (uint i = 0; i < updateables.size(); i++) {
		if (updateables[i]->Is_Enabled()) {
			updateables[i]->Update(in_dt);
		}
	}
}

void PerFrameUpdateManager::Add(PerFrameUpdateableObject* in_updateable) {
	updateables.push_back(in_updateable);
}

void PerFrameUpdateManager::Remove(PerFrameUpdateableObject* in_updateable) {
	auto position = std::find(updateables.begin(), updateables.end(), in_updateable);
	if (position != updateables.end()) {
		updateables.erase(position);
	}
}
