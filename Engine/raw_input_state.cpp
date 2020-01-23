#include "raw_input_state.h"

void RawInputState::processRawEvent(const RawInputEvent& i_event) {
	mKeyboard.processRawEvent(i_event);
	mMouse.processRawEvent(i_event);
}
