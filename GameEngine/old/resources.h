#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>
#include "component.h"

class Resource {
public:
	string name;

			Resource	(string in_name);
	virtual ~Resource	();
};

class ResourceManager {
private:
	std::map<string, void*> resources;

public:
				ResourceManager	();
				~ResourceManager();
	void		Add				(Resource* in_resource);
	void		Remove			(Resource* in_resource);
	Resource*	Get				(string in_name);
};

#endif
