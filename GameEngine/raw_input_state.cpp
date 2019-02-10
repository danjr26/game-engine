#include "raw_input_state.h"

void RawInputState::Process_Raw_Event(const RawInputEvent& in_event) {
	mKeyboard.Process_Raw_Event(in_event);
	mMouse.Process_Raw_Event(in_event);
}
