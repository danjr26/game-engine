#include "keyboard_state_identifier.h"

KeyboardStateIdentifier::KeyboardStateIdentifier(RawInputEvent::Type i_type, Key i_key, InputStateChange i_stateChange, bool i_acceptRepeat) :
	mType(i_type),
	mKey(i_key),
	mStateChange(i_stateChange),
	mAcceptRepeat(i_acceptRepeat)
{}

InputStateChange KeyboardStateIdentifier::operator()(const RawInputEvent& i_event, const RawInputState& i_state) const {
	return (mType == i_event.mType && mKey == i_event.mKeyboard.mKey && ((i_event.mKeyboard.mRepeat) ? mAcceptRepeat : true)) ? 
		mStateChange : InputStateChange::no_change;
}
