#include "keyboard_action_identifier.h"

KeyboardActionIdentifier::KeyboardActionIdentifier(RawInputEvent::Type in_type, Key in_key, bool in_acceptRepeat) :
	type(in_type),
	key(in_key),
	acceptRepeat(false)
{}

bool KeyboardActionIdentifier::operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
	return type == in_event.type && key == in_event.keyboard.key && ((in_event.keyboard.repeat) ? acceptRepeat : true);
}