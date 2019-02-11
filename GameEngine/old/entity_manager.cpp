#include "entity_manager.h"

EntityManager::EntityManager() :
	entities(1024, offsetof(Entity, entityManagerArrayIndex)) 
{}

EntityManager::~EntityManager() {
	std::lock_guard<std::mutex> lock(mutex);
	while (entities.Size() > 0)
		delete entities[0];
}

void EntityManager::Add(Entity* in_entity) {
	std::lock_guard<std::mutex> lock(mutex);
	entities.Add(in_entity);
}

void EntityManager::Remove(Entity* in_entity) {
	std::lock_guard<std::mutex> lock(mutex);
	entities.CustomArray<Entity*>::Remove(in_entity, false);
}

IndexedArray<Entity> const& EntityManager::List() const {
	std::lock_guard<std::mutex> lock(mutex);
	return entities;
}