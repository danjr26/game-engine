#include "mouse_action_identifier.h"

MouseActionIdentifier::MouseActionIdentifier(RawInputEvent::Type in_type, MouseButton in_button) :
	type(in_type),
	button(in_button)
{}

bool MouseActionIdentifier::operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
	return type == in_event.type && button == in_event.mouse.button;
}


