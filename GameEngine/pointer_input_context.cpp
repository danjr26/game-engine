#include "pointer_input_context.h"

PointerInputContext::PointerInputContext() :
	InputContext(Action::count, State::count, Range::count) 
{
	
}

bool PointerInputContext::processRawEvent(const RawInputEvent& in_event) {
	return autoUpdate(in_event);
}