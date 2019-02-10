#include "mouse_state_identifier.h"

MouseStateIdentifier::MouseStateIdentifier(RawInputEvent::Type in_type, MouseButton in_button, InputStateChange in_stateChange) :
	mType(in_type),
	mButton(in_button),
	mStateChange(in_stateChange)
{}

InputStateChange MouseStateIdentifier::operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
	return (mType == in_event.mType && mButton == in_event.mMouse.mButton) ? mStateChange : InputStateChange::no_change;
}
