#include "mouse_state_identifier.h"

MouseStateIdentifier::MouseStateIdentifier(RawInputEvent::Type in_type, MouseButton in_button, InputStateChange in_stateChange) :
	type(in_type),
	button(in_button),
	stateChange(in_stateChange)
{}

InputStateChange MouseStateIdentifier::operator()(const RawInputEvent& in_event, const RawInputState& in_state) const {
	return (type == in_event.type && button == in_event.mouse.button) ? stateChange : InputStateChange::no_change;
}
