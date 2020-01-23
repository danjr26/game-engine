#include "mouse_action_identifier.h"

MouseActionIdentifier::MouseActionIdentifier(RawInputEvent::Type i_type, MouseButton i_button) :
	mType(i_type),
	mButton(i_button)
{}

bool MouseActionIdentifier::operator()(const RawInputEvent& i_event, const RawInputState& i_state) const {
	return mType == i_event.mType && mButton == i_event.mMouse.mButton;
}


