#include "filtered_object.h"

FilteredObject::FilteredObject() :
filter(0ull)
{}

FilteredObject::FilteredObject(ubyte in_filter) :
	filter(1ull << in_filter)
{}

void FilteredObject::Add_Filter(ubyte in_filter) {
	Set_Bit<ullong>(filter, 1ull << in_filter, true);
}

void FilteredObject::Remove_Filter(ubyte in_filter) {
	Set_Bit<ullong>(filter, 1ull << in_filter, false);
}

void FilteredObject::Remove_All_Filters() {
	filter = 0ull;
}

bool FilteredObject::Has_Filter(ubyte in_filter) {
	return filter & (1ull << in_filter);
}


