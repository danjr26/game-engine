#include "input_context.h"
#include "game_engine.h"
#include <algorithm>

InputContext::InputContext() :
	mBindings(),
	mStates(),
	mRanges(),
	mPriority(0.0f)
{}

float InputContext::getPriority() const {
	return mPriority;
}

void InputContext::setPriority(float i_priority) {
	mPriority = i_priority;
}

InputEvent InputContext::autoTranslateAction(InputBindings::Iterator& i_iter) {
	InputEvent _event;
	_event.mContext = this;
	_event.mType = InputEvent::Type::action;
	_event.mMessage = mBindings[i_iter];
	return _event;
}

InputEvent InputContext::autoTranslateState(InputBindings::Iterator& i_iter, bool i_newValue) {
	InputEvent _event;
	_event.mContext = this;
	_event.mType = InputEvent::Type::state_change;
	_event.mMessage = mBindings[i_iter];
	_event.mState.oldValue = mStates[_event.mMessage];
	_event.mState.newValue = i_newValue;
	return _event;
}

InputEvent InputContext::autoTranslateRange(InputBindings::Iterator& i_iter, float i_newValue) {
	InputEvent _event;
	_event.mContext = this;
	_event.mType = InputEvent::Type::range_change;
	_event.mMessage = mBindings[i_iter];
	_event.mRange.oldValue = mRanges[_event.mMessage];
	_event.mRange.newValue = i_newValue;
	return _event;
}

bool InputContext::autoProcessAction(InputBindings::Iterator& i_iter) {
	return distributeEvent(autoTranslateAction(i_iter));
}

bool InputContext::autoProcessState(InputBindings::Iterator& i_iter) {
	InputStateChange stateChange = mBindings.evaluateState(i_iter);
	bool newValue;
	switch (stateChange) {
	case InputStateChange::no_change:
		newValue = mStates[mBindings[i_iter]];
		break;
	case InputStateChange::turn_on:
		newValue = true;
		break;
	case InputStateChange::turn_off:
		newValue = false;
		break;
	case InputStateChange::toggle:
		newValue = !mStates[mBindings[i_iter]];
		break;
	}
	bool eaten = distributeEvent(autoTranslateState(i_iter, newValue));
	mStates[mBindings[i_iter]] = newValue;
	return eaten;
}

bool InputContext::autoProcessRange(InputBindings::Iterator& i_iter) {
	float newValue = mBindings.evaluateRange(i_iter);
	bool eaten = distributeEvent(autoTranslateRange(i_iter, newValue));
	mRanges[mBindings[i_iter]] = newValue;
	return eaten;
}

bool InputContext::autoUpdateActions(const RawInputEvent& i_event) {
	bool out = false;
	for (auto iter = mBindings.iterateActions(i_event, GE.input().getRawState()); iter; ++iter) {
		out = autoProcessAction(iter) || out;
	}
	return out;
}

bool InputContext::autoUpdateStates(const RawInputEvent& i_event) {
	bool out = false;
	for (auto iter = mBindings.iterateStates(i_event, GE.input().getRawState()); iter; ++iter) {
		out = autoProcessState(iter) || out;
	}
	return out;
}

bool InputContext::autoUpdateRanges(const RawInputEvent& i_event) {
	bool out = false;
	for (auto iter = mBindings.iterateRanges(i_event, GE.input().getRawState()); iter; ++iter) {
		out = autoProcessRange(iter) || out;
	}
	return out;
}

bool InputContext::autoUpdate(const RawInputEvent& i_event) {
	bool out = false;
	out = out || autoUpdateActions(i_event);
	out = out || autoUpdateStates(i_event);
	out = out || autoUpdateRanges(i_event);
	return out;
}

bool InputContext::distributeEvent(const InputEvent& i_event) {
	for (uint i = 0; i < mListeners.size(); i++) {
		if (mListeners[i]->postEvent(i_event)) return true;
	}
	return false;
}

bool InputContext::processRawEvent(const RawInputEvent& i_event) {
	return autoUpdate(i_event);
}

bool InputContext::getState(uint i_index) {
	return mStates[i_index];
}

float InputContext::getRange(uint i_index) {
	return mRanges[i_index];
}

void InputContext::add(InputListener* i_listener) {
	mListeners.push_back(i_listener);
	sort();
}

void InputContext::remove(InputListener* i_listener) {
	auto position = std::find(mListeners.begin(), mListeners.end(), i_listener);
	if (position != mListeners.end()) {
		mListeners.erase(position);
	}
	sort();
}

void InputContext::sort() {
	std::sort(mListeners.begin(), mListeners.end(), InputListener::comparePointers);
}
