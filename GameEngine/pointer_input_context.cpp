#include "pointer_input_context.h"

PointerInputContext::PointerInputContext() :
	InputContext(Action::count, State::count, Range::count) 
{
	
}

bool PointerInputContext::Process_Raw_Event(const RawInputEvent& in_event) {
	return Auto_Update(in_event);
}