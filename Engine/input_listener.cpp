#include "input_listener.h"
#include "input_context.h"
#include "exceptions.h"

InputListener::InputListener(InputContext* i_context, uint i_level, double i_depth) :
	mContext(i_context),
	mLevel(i_level),
	mDepth(i_depth),
	mReceiver([](const InputEvent& i_event) -> Reception {return { true, false }; })
{
	if (i_context == nullptr) throw InvalidArgumentException("input mContext was null");
	i_context->add(this);
}

InputListener::~InputListener() {
	mContext->remove(this);
}

bool InputListener::postEvent(const InputEvent& i_event) {
	Reception reception = mReceiver(i_event);
	if (reception.mTaken) {
		mEventQueue.insert(mEventQueue.begin(), i_event);
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

void InputListener::setLevel(uint i_level) {
	mLevel = i_level;
	mContext->sort();
}

double InputListener::getDepth() const {
	return mDepth;
}

void InputListener::setDepth(double i_depth) {
	mDepth = i_depth;
	mContext->sort();
}

void InputListener::setReceiver(const std::function<Reception(const InputEvent&)>& i_receiver) {
	mReceiver = i_receiver;
}

bool InputListener::compare(const InputListener& i_listener1, const InputListener& i_listener2) {
	if (i_listener1.getLevel() > i_listener2.getLevel()) {
		return true;
	}
	else if (i_listener1.getLevel() < i_listener2.getLevel()) {
		return false;
	}
	else {
		return i_listener1.getDepth() > i_listener2.getDepth();
	}
}

bool InputListener::comparePointers(const InputListener* i_listener1, const InputListener* i_listener2) {
	return compare(*i_listener1, *i_listener2);
}

InputContext* InputListener::getContext() {
	return mContext;
}
