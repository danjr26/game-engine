#include "raw_input_state.h"

void RawInputState::processRawEvent(const RawInputEvent& in_event) {
	mKeyboard.processRawEvent(in_event);
	mMouse.processRawEvent(in_event);
}
