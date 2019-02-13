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
	in_context->add(this);
}

InputListener::~InputListener() {
	mContext->remove(this);
}

bool InputListener::postEvent(const InputEvent& in_event) {
	Reception reception = mReceiver(in_event);
	if (reception.mTaken) {
		mEventQueue.insert(mEventQueue.begin(), in_event);
	}
	return reception.mEaten;
}

InputEvent InputListener::popEvent() {
	InputEvent _event = mEventQueue.back();
	mEventQueue.pop_back();
	return _event;
}

uint InputListener::getNumberEvents() {
	return (uint)mEventQueue.size();
}

void InputListener::clearEvents() {
	mEventQueue.clear();
}

uint InputListener::getLevel() const {
	return mLevel;
}

void InputListener::setLevel(uint in_level) {
	mLevel = in_level;
	mContext->sort();
}

double InputListener::getDepth() const {
	return mDepth;
}

void InputListener::setDepth(double in_depth) {
	mDepth = in_depth;
	mContext->sort();
}

void InputListener::setReceiver(const std::function<Reception(const InputEvent&)>& in_receiver) {
	mReceiver = in_receiver;
}

bool InputListener::compare(const InputListener& in_listener1, const InputListener& in_listener2) {
	if (in_listener1.getLevel() > in_listener2.getLevel()) {
		return true;
	}
	else if (in_listener1.getLevel() < in_listener2.getLevel()) {
		return false;
	}
	else {
		return in_listener1.getDepth() > in_listener2.getDepth();
	}
}

bool InputListener::comparePointers(const InputListener* in_listener1, const InputListener* in_listener2) {
	return compare(*in_listener1, *in_listener2);
}

InputContext* InputListener::getContext() {
	return mContext;
}
