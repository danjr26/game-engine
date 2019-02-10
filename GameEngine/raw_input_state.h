#ifndef RAW_INPUT_STATE_H
#define RAW_INPUT_STATE_H

#include "raw_input_event.h"
#include "keyboard_state.h"
#include "mouse_state.h"

struct RawInputState {
	KeyboardState mKeyboard;
	MouseState mMouse;

	void processRawEvent(const RawInputEvent& in_event);
};

#endif