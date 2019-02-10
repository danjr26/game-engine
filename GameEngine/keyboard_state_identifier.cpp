#include "keyboard_state_identifier.h"

KeyboardStateIdentifier::KeyboardStateIdentifier(RawInputEvent::Type in_type, Key in_key, InputStateChange in_stateChange, bool in_acceptRepeat) :
	mType(in_type),
	mKey(in_key),
	mStateChange(in_stateChange),
	mAcceptRepeat(in_acceptRepeat)
{}

InputStateChange KeyboardStateIdentifier::operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
	return (mType == in_event.mType && mKey == in_event.mKeyboard.mKey && ((in_event.mKeyboard.mRepeat) ? mAcceptRepeat : true)) ? 
		mStateChange : InputStateChange::no_change;
}
