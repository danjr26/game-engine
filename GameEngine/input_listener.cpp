#include "input_listener.h"
#include "input_context.h"
#include "exceptions.h"

InputListener::InputListener(InputContext* in_context, uint in_level, double in_depth) :
	context(in_context),
	level(in_level),
	depth(in_depth),
	receiver([](const InputEvent& in_event) -> Reception {return { true, false }; })
{
	if (in_context == nullptr) throw InvalidArgumentException("input context was null");
	in_context->Add(this);
}

bool InputListener::Post_Event(const InputEvent& in_event) {
	Reception reception = receiver(in_event);
	if (reception.taken) {
		eventQueue.insert(eventQueue.begin(), in_event);
	}
	return reception.eaten;
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

uint InputListener::Get_Level() const {
	return level;
}

void InputListener::Set_Level(uint in_level) {
	level = in_level;
	context->Sort();
}

double InputListener::Get_Depth() const {
	return depth;
}

void InputListener::Set_Depth(double in_depth) {
	depth = in_depth;
	context->Sort();
}

void InputListener::Set_Receiver(const std::function<Reception(const InputEvent&)>& in_receiver) {
	receiver = in_receiver;
}

bool InputListener::Compare(const InputListener& in_listener1, const InputListener& in_listener2) {
	if (in_listener1.Get_Level() > in_listener2.Get_Level()) {
		return true;
	}
	else if (in_listener1.Get_Level() < in_listener2.Get_Level()) {
		return false;
	}
	else {
		return in_listener1.Get_Depth() > in_listener2.Get_Depth();
	}
}

bool InputListener::Compare_Pointers(const InputListener* in_listener1, const InputListener* in_listener2) {
	return Compare(*in_listener1, *in_listener2);
}

InputContext* InputListener::Get_Context() {
	return context;
}
