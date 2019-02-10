#include "input_listener.h"
#include "input_context.h"
#include "exceptions.h"

InputListener::InputListener(InputContext* in_context, uint in_level, double in_depth) :
	mContext(in_context),
	mLevel(in_level),
	mDepth(in_depth),
	mReceiver([](const InputEvent& in_event) -> Reception {return { true, false }; })
{
	if (in_context == nullptr) throw InvalidArgumentException("input mContext was null");
	in_context->Add(this);
}

bool InputListener::Post_Event(const InputEvent& in_event) {
	Reception reception = mReceiver(in_event);
	if (reception.mTaken) {
		mEventQueue.insert(mEventQueue.begin(), in_event);
	}
	return reception.mEaten;
}

InputEvent InputListener::Pop_Event() {
	InputEvent _event = mEventQueue.back();
	mEventQueue.pop_back();
	return _event;
}

uint InputListener::Get_Number_Events() {
	return (uint)mEventQueue.size();
}

void InputListener::Clear_Events() {
	mEventQueue.clear();
}

uint InputListener::Get_Level() const {
	return mLevel;
}

void InputListener::Set_Level(uint in_level) {
	mLevel = in_level;
	mContext->Sort();
}

double InputListener::Get_Depth() const {
	return mDepth;
}

void InputListener::Set_Depth(double in_depth) {
	mDepth = in_depth;
	mContext->Sort();
}

void InputListener::Set_Receiver(const std::function<Reception(const InputEvent&)>& in_receiver) {
	mReceiver = in_receiver;
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
	return mContext;
}
