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

	KeyboardActionIdentifier(RawInputEvent::Type i_type, Key i_key, bool i_acceptRepeat = false);
	KeyboardActionIdentifier(RawInputEvent::Type i_type, char i_character, bool i_acceptRepeat = false);
	bool operator()(const RawInputEvent& i_event, const RawInputState& i_state) const;
};

#endif