#ifndef KEYBOARD_ACTION_IDENTIFIER_H
#define KEYBOARD_ACTION_IDENTIFIER_H

#include "key.h"
#include "raw_input_event.h"
#include "raw_input_state.h"

struct KeyboardActionIdentifier {
	RawInputEvent::Type mType;
	union {
		Key mKey;
		char mCharacter;
	};
	bool mAcceptRepeat;

	KeyboardActionIdentifier(RawInputEvent::Type in_type, Key in_key, bool in_acceptRepeat = false);
	KeyboardActionIdentifier(RawInputEvent::Type in_type, char in_character, bool in_acceptRepeat = false);
	bool operator()(const RawInputEvent& in_event, const RawInputState& in_state) const;
};

#endif