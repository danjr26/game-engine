#include "disableable_object.h"

DisableableObject::DisableableObject() :
	isEnabled(true)
{}

void DisableableObject::Enable() {
	isEnabled = true;
}

void DisableableObject::Disable() {
	isEnabled = false;
}

bool DisableableObject::Is_Enabled() {
	return isEnabled;
}