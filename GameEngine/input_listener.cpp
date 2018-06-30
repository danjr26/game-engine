#include "input_listener.h"
#include "input_context.h"

InputListener::InputListener(InputContext* in_context) :
	context(in_context) {

	if (in_context == nullptr) {
		Log::main("error: null input context");
		exit(-1);
	}

	in_context->Add(this);
}

void InputListener::Post_Event(InputEvent in_event) {
	eventQueue.insert(eventQueue.begin(), in_event);
}

InputEvent InputListener::Pop_Event() {
	InputEvent _event = eventQueue.back();
	eventQueue.pop_back();
	return _event;
}

uint InputListener::Get_Number_Events() {
	return eventQueue.size();
}

void InputListener::Clear_Events() {
	eventQueue.clear();
}

InputContext* InputListener::Get_Context() {
	return context;
}
