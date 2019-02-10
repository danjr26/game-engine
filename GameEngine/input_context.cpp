#include "input_context.h"
#include "game_engine.h"
#include <algorithm>

InputContext::InputContext(uint in_nActions, uint in_nStates, uint in_nRanges) :
	mBindings(in_nActions, in_nStates, in_nRanges),
	mStates(in_nStates),
	mRanges(in_nRanges)
{}

InputEvent InputContext::autoTranslateAction(InputBindings::Iterator & in_iter) {
	InputEvent _event;
	_event.mContext = this;
	_event.mType = InputEvent::Type::action;
	_event.mMessage = mBindings[in_iter];
	return _event;
}

InputEvent InputContext::autoTranslateState(InputBindings::Iterator& in_iter, bool in_newValue) {
	InputEvent _event;
	_event.mContext = this;
	_event.mType = InputEvent::Type::state_change;
	_event.mMessage = mBindings[in_iter];
	_event.mState.oldValue = mStates[_event.mMessage];
	_event.mState.newValue = in_newValue;
	return _event;
}

InputEvent InputContext::autoTranslateRange(InputBindings::Iterator& in_iter, float in_newValue) {
	InputEvent _event;
	_event.mContext = this;
	_event.mType = InputEvent::Type::range_change;
	_event.mMessage = mBindings[in_iter];
	_event.mRange.oldValue = mRanges[_event.mMessage];
	_event.mRange.newValue = in_newValue;
	return _event;
}

bool InputContext::autoProcessAction(InputBindings::Iterator& in_iter) {
	return distributeEvent(autoTranslateAction(in_iter));
}

bool InputContext::autoProcessState(InputBindings::Iterator& in_iter) {
	InputStateChange stateChange = mBindings.evaluateState(in_iter);
	bool newValue;
	switch (stateChange) {
	case InputStateChange::no_change:
		newValue = mStates[mBindings[in_iter]];
		break;
	case InputStateChange::turn_on:
		newValue = true;
		break;
	case InputStateChange::turn_off:
		newValue = false;
		break;
	case InputStateChange::toggle:
		newValue = !mStates[mBindings[in_iter]];
		break;
	}
	bool eaten = distributeEvent(autoTranslateState(in_iter, newValue));
	mStates[mBindings[in_iter]] = newValue;
	return eaten;
}

bool InputContext::autoProcessRange(InputBindings::Iterator& in_iter) {
	float newValue = mBindings.evaluateRange(in_iter);
	bool eaten = distributeEvent(autoTranslateRange(in_iter, newValue));
	mRanges[mBindings[in_iter]] = newValue;
	return eaten;
}

bool InputContext::autoUpdateActions(const RawInputEvent& in_event) {
	bool out = false;
	for (auto iter = mBindings.iterateActions(in_event, GE.input().getRawState()); iter; ++iter) {
		out = autoProcessAction(iter) || out;
	}
	return out;
}

bool InputContext::autoUpdateStates(const RawInputEvent& in_event) {
	bool out = false;
	for (auto iter = mBindings.iterateStates(in_event, GE.input().getRawState()); iter; ++iter) {
		out = autoProcessState(iter) || out;
	}
	return out;
}

bool InputContext::autoUpdateRanges(const RawInputEvent& in_event) {
	bool out = false;
	for (auto iter = mBindings.iterateRanges(in_event, GE.input().getRawState()); iter; ++iter) {
		out = autoProcessRange(iter) || out;
	}
	return out;
}

bool InputContext::autoUpdate(const RawInputEvent& in_event) {
	return (
		autoUpdateActions(in_event) || 
		autoUpdateStates(in_event) ||
		autoUpdateRanges(in_event)
	);
}

bool InputContext::distributeEvent(const InputEvent& in_event) {
	for (uint i = 0; i < mListeners.size(); i++) {
		if (mListeners[i]->postEvent(in_event)) return true;
	}
	return false;
}

bool InputContext::processRawEvent(const RawInputEvent& in_event) {
	return autoUpdate(in_event);
}

bool InputContext::getState(uint in_index) {
	return mStates[in_index];
}

float InputContext::getRange(uint in_index) {
	return mRanges[in_index];
}

void InputContext::add(InputListener* in_listener) {
	mListeners.push_back(in_listener);
	sort();
}

void InputContext::remove(InputListener* in_listener) {
	auto position = std::find(mListeners.begin(), mListeners.end(), in_listener);
	if (position != mListeners.end()) {
		mListeners.erase(position);
	}
	sort();
}

void InputContext::sort() {
	std::sort(mListeners.begin(), mListeners.end(), InputListener::comparePointers);
}
