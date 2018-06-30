#ifndef RAW_INPUT_STATE_H
#define RAW_INPUT_STATE_H

#include "raw_input_event.h"
#include "keyboard_state.h"
#include "mouse_state.h"

struct RawInputState {
	KeyboardState keyboard;
	MouseState mouse;

	void Process_Raw_Event(const RawInputEvent& in_event);
};

#endif