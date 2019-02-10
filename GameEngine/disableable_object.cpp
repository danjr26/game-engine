#include "disableable_object.h"

DisableableObject::DisableableObject() :
	mIsEnabled(true)
{}

void DisableableObject::Enable() {
	mIsEnabled = true;
}

void DisableableObject::Disable() {
	mIsEnabled = false;
}

bool DisableableObject::Is_Enabled() {
	return mIsEnabled;
}