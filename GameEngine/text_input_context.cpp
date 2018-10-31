#include "text_input_context.h"

TextInputContext::TextInputContext() :
	InputContext(Action::count, 0, 0)
{}

bool TextInputContext::Process_Raw_Event(const RawInputEvent& in_event) {
	return Auto_Update(in_event);
}
