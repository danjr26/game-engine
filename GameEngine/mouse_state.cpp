#include "mouse_state.h"

MouseState::MouseState() :
	mButtons((ubyte)MouseButton::count) {

	for (uint i = 0; i < (ubyte)MouseButton::count; i++) {
		mButtons[i] = false;
	}
}

bool MouseState::isDown(MouseButton in_button) {
	return mButtons[(ubyte) in_button];
}

void MouseState::setButton(MouseButton in_button, bool in_value) {
	mButtons[(ubyte)in_button] = in_value;
}

void MouseState::processRawEvent(RawInputEvent in_event) {
	switch (in_event.mType) {
	case RawInputEvent::Type::mouse_down:
		setButton(in_event.mMouse.mButton, true);
		break;
	case RawInputEvent::Type::mouse_up:
		setButton(in_event.mMouse.mButton, false);
		break;
	}
}
