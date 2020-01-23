#include "text_input_context.h"

TextInputContext::TextInputContext()
{}

bool TextInputContext::processRawEvent(const RawInputEvent& i_event) {
	return autoUpdate(i_event);
}
