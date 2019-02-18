#ifndef DESTRUCTION_MANAGER_H
#define DESTRUCTION_MANAGER_H

#include "destructable_object.h"
#include <set>

class DestructionManager {
private:
	std::set<DestructableObject*> mObjects;

public:
	DestructionManager();

	void add(DestructableObject* in_object);
	bool has(DestructableObject* in_object);

	void update();
};

#endif