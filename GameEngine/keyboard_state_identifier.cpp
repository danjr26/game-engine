#include "keyboard_state_identifier.h"

KeyboardStateIdentifier::KeyboardStateIdentifier(RawInputEvent::Type in_type, Key in_key, InputStateChange in_stateChange, bool in_acceptRepeat) :
	type(in_type),
	key(in_key),
	stateChange(in_stateChange),
	acceptRepeat(in_acceptRepeat)
{}

InputStateChange KeyboardStateIdentifier::operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
	return (type == in_event.type && key == in_event.keyboard.key && ((in_event.keyboard.repeat) ? acceptRepeat : true)) ? 
		stateChange : InputStateChange::no_change;
}
