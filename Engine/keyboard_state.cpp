#include "keyboard_state.h"

KeyboardState::KeyboardState() :
	mKeyStates((ubyte)Key::count) {

	for (uint i = 0; i < (ubyte)Key::count; i++) {
		mKeyStates[i] = false;
	}
}

bool KeyboardState::isDown(Key i_key) const {
	return mKeyStates[(ubyte)i_key];
}

void KeyboardState::setKey(Key i_key, bool i_value) {
	mKeyStates[(ubyte)i_key] = i_value;
}

void KeyboardState::processRawEvent(const RawInputEvent& i_event) {
	switch (i_event.mType) {
	case RawInputEvent::Type::key_down:
		setKey(i_event.mKeyboard.mKey, true);
		break;
	case RawInputEvent::Type::key_up:
		setKey(i_event.mKeyboard.mKey, false);
		break;
	}
}
