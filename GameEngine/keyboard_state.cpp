#include "keyboard_state.h"

KeyboardState::KeyboardState() :
	mKeyStates((ubyte)Key::count) {

	for (uint i = 0; i < (ubyte)Key::count; i++) {
		mKeyStates[i] = false;
	}
}

bool KeyboardState::isDown(Key in_key) const {
	return mKeyStates[(ubyte)in_key];
}

void KeyboardState::setKey(Key in_key, bool in_value) {
	mKeyStates[(ubyte)in_key] = in_value;
}

void KeyboardState::processRawEvent(const RawInputEvent& in_event) {
	switch (in_event.mType) {
	case RawInputEvent::Type::key_down:
		setKey(in_event.mKeyboard.mKey, true);
		break;
	case RawInputEvent::Type::key_up:
		setKey(in_event.mKeyboard.mKey, false);
		break;
	}
}
