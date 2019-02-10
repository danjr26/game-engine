#include "filtered_object.h"

FilteredObject::FilteredObject() :
mFilter(0ull)
{}

FilteredObject::FilteredObject(ubyte in_filter) :
	mFilter(1ull << in_filter)
{}

void FilteredObject::Add_Filter(ubyte in_filter) {
	Set_Bit<ullong>(mFilter, 1ull << in_filter, true);
}

void FilteredObject::Remove_Filter(ubyte in_filter) {
	Set_Bit<ullong>(mFilter, 1ull << in_filter, false);
}

void FilteredObject::Remove_All_Filters() {
	mFilter = 0ull;
}

bool FilteredObject::Has_Filter(ubyte in_filter) {
	return mFilter & (1ull << in_filter);
}


