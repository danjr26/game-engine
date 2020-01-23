#ifndef MOUSE_ACTION_IDENTIFIER_H
#define MOUSE_ACTION_IDENTIFIER_H

#include "mouse_button.h"
#include "raw_input_event.h"
#include "raw_input_state.h"

struct MouseActionIdentifier {
	RawInputEvent::Type mType;
	MouseButton mButton;

	MouseActionIdentifier(RawInputEvent::Type i_type, MouseButton i_button);
	bool operator()(const RawInputEvent& i_event, const RawInputState& i_state) const;
};

#endif
