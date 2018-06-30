#include "common_object.h"

SharedObject::SharedObject(const std::string& in_name) :
	name(in_name) 
{}

std::string SharedObject::Get_Name() {
	return name;
}
