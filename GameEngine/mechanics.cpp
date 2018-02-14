#include "mechanics.h"
#include "framework.h"

MechanicsManager::MechanicsManager() :
components(CAPACITY, offsetof(MechanicalComponent, mechanicsManagerArrayIndex)) 
{}

MechanicsManager::~MechanicsManager() {
	while(components.Size())
		delete components[0];
}

void MechanicsManager::Add(MechanicalComponent* component) {
	components.Add(component);
}

void MechanicsManager::Remove(MechanicalComponent* component) {
	components.Remove(component);
}

