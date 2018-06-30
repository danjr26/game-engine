#ifndef MOUSE_ACTION_IDENTIFIER_H
#define MOUSE_ACTION_IDENTIFIER_H

#include "mouse_button.h"
#include "raw_input_event.h"
#include "raw_input_state.h"

struct MouseActionIdentifier {
	RawInputEvent::Type type;
	MouseButton button;

	MouseActionIdentifier(RawInputEvent::Type in_type, MouseButton in_button);
	bool operator()(const RawInputEvent& in_event, const RawInputState& in_state) const;
};

#endif
