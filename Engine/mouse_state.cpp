#include "mouse_state.h"

MouseState::MouseState() :
	mButtons((ubyte)MouseButton::count) {

	for (uint i = 0; i < (ubyte)MouseButton::count; i++) {
		mButtons[i] = false;
	}
}

bool MouseState::isDown(MouseButton i_button) {
	return mButtons[(ubyte) i_button];
}

void MouseState::setButton(MouseButton i_button, bool i_value) {
	mButtons[(ubyte)i_button] = i_value;
}

void MouseState::processRawEvent(RawInputEvent i_event) {
	switch (i_event.mType) {
	case RawInputEvent::Type::mouse_down:
		setButton(i_event.mMouse.mButton, true);
		break;
	case RawInputEvent::Type::mouse_up:
		setButton(i_event.mMouse.mButton, false);
		break;
	}
}
