#include "mouse_action_identifier.h"

MouseActionIdentifier::MouseActionIdentifier(RawInputEvent::Type in_type, MouseButton in_button) :
	mType(in_type),
	mButton(in_button)
{}

bool MouseActionIdentifier::operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
	return mType == in_event.mType && mButton == in_event.mMouse.mButton;
}


