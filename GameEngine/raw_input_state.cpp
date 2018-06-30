#include "raw_input_state.h"

void RawInputState::Process_Raw_Event(const RawInputEvent& in_event) {
	keyboard.Process_Raw_Event(in_event);
	mouse.Process_Raw_Event(in_event);
}
