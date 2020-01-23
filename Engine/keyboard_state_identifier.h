#ifndef KEYBOARD_STATE_IDENTIFIER_H
#define KEYBOARD_STATE_IDENTIFIER_H

#include "input_identifiers.h"
#include "key.h"

struct KeyboardStateIdentifier {
	RawInputEvent::Type mType;
	Key mKey;
	InputStateChange mStateChange;
	bool mAcceptRepeat;

	KeyboardStateIdentifier(RawInputEvent::Type i_type, Key i_key, InputStateChange i_stateChange, bool i_acceptRepeat = false);
	InputStateChange operator()(const RawInputEvent& i_event, const RawInputState& i_state) const;
};

#endif