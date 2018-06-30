#ifndef FILTERED_OBJECT_H
#define FILTERED_OBJECT_H

#include "definitions.h"
#include "bit_manipulation.h"
#include <vector>

class FilteredObject {
	friend class FilterQuery;
private:
	ulong filter;

public:
	FilteredObject();
	FilteredObject(ubyte in_filter);
	void Add_Filter(ubyte in_filter);
	void Remove_Filter(ubyte in_filter);
	void Remove_All_Filters();
	bool Has_Filter(ubyte in_filter);
};

#endif