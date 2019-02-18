#ifndef KEYBOARD_STATE_IDENTIFIER_H
#define KEYBOARD_STATE_IDENTIFIER_H

#include "input_identifiers.h"
#include "key.h"

struct KeyboardStateIdentifier {
	RawInputEvent::Type mType;
	Key mKey;
	InputStateChange mStateChange;
	bool mAcceptRepeat;

	KeyboardStateIdentifier(RawInputEvent::Type in_type, Key in_key, InputStateChange in_stateChange, bool in_acceptRepeat = false);
	InputStateChange operator()(const RawInputEvent& in_event, const RawInputState& in_state) const;
};

#endif