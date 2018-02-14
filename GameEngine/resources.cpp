#include "resources.h"
#include "framework.h"

ResourceManager* ResourceManager::active = nullptr;

Resource::Resource(std::string path, std::string name) :
path		(path),
name		(name) {
	GEngine::Get().Resource().resources.Add(this);
}

Resource::~Resource() {
	GEngine::Get().Resource().resources.Remove(this);
}

ResourceManager::ResourceManager() :
resources(256, offsetof(Resource, rmindex)) {
	active = this;
}

ResourceManager::~ResourceManager() 
{}

void ResourceManager::Add(Resource* resource) {
	resources.Add(resource);
}

void ResourceManager::Remove(Resource* resource) {
	resources.Remove(resource);
}

Resource* ResourceManager::Get_Resource(string name) {
	for (int i = 0; i < resources.Size(); i++) {
		if (resources[i]->name == name)
			return resources[i];
	}
	//Die("ResourceManager.Get_Resource: resource not found");
	return nullptr;
}
