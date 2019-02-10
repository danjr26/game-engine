#include "text_input_context.h"

TextInputContext::TextInputContext() :
	InputContext(Action::count, 0, 0)
{}

bool TextInputContext::processRawEvent(const RawInputEvent& in_event) {
	return autoUpdate(in_event);
}
