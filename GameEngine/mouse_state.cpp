#include "mouse_state.h"

MouseState::MouseState() :
	buttons((ubyte)MouseButton::count) {

	for (uint i = 0; i < (ubyte)MouseButton::count; i++) {
		buttons[i] = false;
	}
}

bool MouseState::Is_Down(MouseButton in_button) {
	return buttons[(ubyte) in_button];
}

void MouseState::Set_Button(MouseButton in_button, bool in_value) {
	buttons[(ubyte)in_button] = in_value;
}

void MouseState::Process_Raw_Event(RawInputEvent in_event) {
	switch (in_event.type) {
	case RawInputEvent::Type::mouse_down:
		Set_Button(in_event.mouse.button, true);
		break;
	case RawInputEvent::Type::mouse_up:
		Set_Button(in_event.mouse.button, false);
		break;
	}
}
