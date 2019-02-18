#include "text_input_context.h"

TextInputContext::TextInputContext()
{}

bool TextInputContext::processRawEvent(const RawInputEvent& in_event) {
	return autoUpdate(in_event);
}
