#include "destruction_manager.h"

DestructionManager::DestructionManager() :
	mObjects()
{}

void DestructionManager::add(DestructableObject* in_object) {
	mObjects.insert(in_object);
}

bool DestructionManager::has(DestructableObject* in_object) {
	return mObjects.count(in_object);
}

void DestructionManager::update() {
	for (auto it = mObjects.begin(); it != mObjects.end(); it++) {
		delete *it;
	}

	mObjects.clear();
}
