#include "filtered_object.h"

FilteredObject::FilteredObject() :
mFilter(0ull)
{}

FilteredObject::FilteredObject(ubyte in_filter) :
	mFilter(1ull << in_filter)
{}

void FilteredObject::addFilter(ubyte in_filter) {
	setBit<ullong>(mFilter, 1ull << in_filter, true);
}

void FilteredObject::removeFilter(ubyte in_filter) {
	setBit<ullong>(mFilter, 1ull << in_filter, false);
}

void FilteredObject::removeAllFilters() {
	mFilter = 0ull;
}

bool FilteredObject::hasFilter(ubyte in_filter) {
	return mFilter & (1ull << in_filter);
}


