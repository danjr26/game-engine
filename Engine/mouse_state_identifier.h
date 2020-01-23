#ifndef MOUSE_STATE_IDENTIFIER_H
#define MOUSE_STATE_IDENTIFIER_H

#include "input_identifiers.h"
#include "mouse_button.h"

struct MouseStateIdentifier {
	RawInputEvent::Type mType;
	MouseButton mButton;
	InputStateChange mStateChange;

	MouseStateIdentifier(RawInputEvent::Type i_type, MouseButton i_button, InputStateChange i_stateChange);
	InputStateChange operator()(const RawInputEvent& i_event, const RawInputState& i_state) const;
};

#endif
