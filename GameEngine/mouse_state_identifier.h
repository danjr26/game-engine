#ifndef MOUSE_STATE_IDENTIFIER_H
#define MOUSE_STATE_IDENTIFIER_H

#include "input_identifiers.h"
#include "mouse_button.h"

class MouseStateIdentifier {
	RawInputEvent::Type mType;
	MouseButton mButton;
	InputStateChange mStateChange;

	MouseStateIdentifier(RawInputEvent::Type in_type, MouseButton in_button, InputStateChange in_stateChange);
	InputStateChange operator()(const RawInputEvent& in_event, const RawInputState& in_state) const;
};

#endif
