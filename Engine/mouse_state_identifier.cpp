#include "mouse_state_identifier.h"

MouseStateIdentifier::MouseStateIdentifier(RawInputEvent::Type i_type, MouseButton i_button, InputStateChange i_stateChange) :
	mType(i_type),
	mButton(i_button),
	mStateChange(i_stateChange)
{}

InputStateChange MouseStateIdentifier::operator()(const RawInputEvent& i_event, const RawInputState& i_state) const {
	return (mType == i_event.mType && mButton == i_event.mMouse.mButton) ? mStateChange : InputStateChange::no_change;
}
