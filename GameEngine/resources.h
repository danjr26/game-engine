#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>
#include "component.h"

class Resource {
public:
	uint rmindex;
	std::string path;
	std::string name;

			Resource	(std::string path, std::string name);
	virtual ~Resource	();
};

class ResourceManager {
public:
	static ResourceManager* active;
	IndexedArray<Resource> resources;

				ResourceManager	();
				~ResourceManager();
	void		Add				(Resource* resource);
	void		Remove			(Resource* resource);
	Resource*	Get_Resource	(string name);
};

#endif
