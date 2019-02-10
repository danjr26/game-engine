#include "disableable_object.h"

DisableableObject::DisableableObject() :
	mIsEnabled(true)
{}

void DisableableObject::enable() {
	mIsEnabled = true;
}

void DisableableObject::disable() {
	mIsEnabled = false;
}

bool DisableableObject::isEnabled() {
	return mIsEnabled;
}