#include "filtered_object.h"

FilteredObject::FilteredObject() :
filter(0ul)
{}

FilteredObject::FilteredObject(ubyte in_filter) :
	filter(1ul << in_filter)
{}

void FilteredObject::Add_Filter(ubyte in_filter) {
	Set_Bit<ulong>(filter, 1ul << in_filter, true);
}

void FilteredObject::Remove_Filter(ubyte in_filter) {
	Set_Bit<ulong>(filter, 1ul << in_filter, false);
}

void FilteredObject::Remove_All_Filters() {
	filter = 0ul;
}

bool FilteredObject::Has_Filter(ubyte in_filter) {
	return filter & (1ul << in_filter);
}


