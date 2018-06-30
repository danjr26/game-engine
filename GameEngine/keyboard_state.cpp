#include "keyboard_state.h"

KeyboardState::KeyboardState() :
	keyStates((ubyte)Key::count) {

	for (uint i = 0; i < (ubyte)Key::count; i++) {
		keyStates[i] = false;
	}
}

bool KeyboardState::Is_Down(Key in_key) const {
	return keyStates[(ubyte)in_key];
}

void KeyboardState::Set_Key(Key in_key, bool in_value) {
	keyStates[(ubyte)in_key] = in_value;
}

void KeyboardState::Process_Raw_Event(const RawInputEvent& in_event) {
	switch (in_event.type) {
	case RawInputEvent::Type::key_down:
		Set_Key(in_event.keyboard.key, true);
		break;
	case RawInputEvent::Type::key_up:
		Set_Key(in_event.keyboard.key, false);
		break;
	}
}
