#ifndef FILTERED_OBJECT_H
#define FILTERED_OBJECT_H

#include "definitions.h"
#include "bit_manipulation.h"
#include <vector>

class FilteredObject {
	friend class FilterQuery;
private:
	ullong mFilter;

public:
	static const ubyte maxFilters = sizeof(ullong);

public:
	FilteredObject();
	FilteredObject(ubyte in_filter);
	void addFilter(ubyte in_filter);
	void removeFilter(ubyte in_filter);
	void removeAllFilters();
	bool hasFilter(ubyte in_filter);
};

#endif