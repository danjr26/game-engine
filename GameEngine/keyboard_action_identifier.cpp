#include "keyboard_action_identifier.h"

KeyboardActionIdentifier::KeyboardActionIdentifier(RawInputEvent::Type in_type, Key in_key, bool in_acceptRepeat) :
	type(in_type),
	key(in_key),
	acceptRepeat(in_acceptRepeat)
{}

KeyboardActionIdentifier::KeyboardActionIdentifier(RawInputEvent::Type in_type, char in_character, bool in_acceptRepeat) :
	type(in_type),
	character(in_character),
	acceptRepeat(in_acceptRepeat)
{}

bool KeyboardActionIdentifier::operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
	if (type == in_event.type) {
		switch (type) {
		case RawInputEvent::character:
			return character == in_event.keyboard.character && ((in_event.keyboard.repeat) ? acceptRepeat : true);
		case RawInputEvent::key_down:
		case RawInputEvent::key_up:
			return key == in_event.keyboard.key && ((in_event.keyboard.repeat) ? acceptRepeat : true);
		}
	}
	return false;
}