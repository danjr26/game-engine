#include "destruction_manager.h"

DestructionManager::DestructionManager() :
	mObjects()
{}

void DestructionManager::add(DestructableObject* i_object) {
	mObjects.insert(i_object);
}

bool DestructionManager::has(DestructableObject* i_object) {
	return mObjects.count(i_object);
}

void DestructionManager::update() {
	for (auto it = mObjects.begin(); it != mObjects.end(); it++) {
		delete *it;
	}

	mObjects.clear();
}
