#include "input_bindings.h"
#include <cmath>

InputBindings::InputBindings() 
{}

uint InputBindings::getNumberActions() const {
	return (uint)mActions.size();
}

uint InputBindings::getNumberStates() const {
	return (uint)mStates.size();
}

uint InputBindings::getNumberRanges() const {
	return (uint)mRanges.size();
}

void InputBindings::bindAction(key_t in_slot, action_t in_action) {
	mActions.insert(std::pair<uint, InputActionIdentifier>(in_slot, in_action));
}

void InputBindings::bindState(key_t in_slot, state_t in_state) {
	mStates.insert(std::pair<uint, InputStateIdentifier>(in_slot, in_state));
}

void InputBindings::bindRange(key_t in_slot, range_t in_range) {
	mRanges.insert(std::pair<uint, InputRangeIdentifier>(in_slot, in_range));
}

void InputBindings::clearAction(key_t in_slot) {
	mActions.erase(in_slot);
}

void InputBindings::clearState(key_t in_slot) {
	mStates.erase(in_slot);
}

void InputBindings::clearRange(key_t in_slot) {
	mRanges.erase(in_slot);
}

bool InputBindings::evaluateAction(key_t in_slot, const RawInputEvent& in_event, const RawInputState& in_state) {
	auto search = mActions.equal_range(in_slot);
	for (auto it = search.first; it != search.second; it++) {
		if (it->second(in_event, in_state)) return true;
	}
	return false;
}

InputStateChange InputBindings::evaluateState(key_t in_slot, const RawInputEvent& in_event, const RawInputState& in_state) {
	auto search = mStates.equal_range(in_slot);
	for (auto it = search.first; it != search.second; it++) {
		InputStateChange change = it->second(in_event, in_state);
		if (change != InputStateChange::no_change) return change;
	}
	return InputStateChange::no_change;
}

float InputBindings::evaluateRange(key_t in_slot, const RawInputEvent& in_event, const RawInputState& in_state) {
	auto search = mRanges.equal_range(in_slot);
	for (auto it = search.first; it != search.second; it++) {
		float value = it->second(in_event, in_state);
		if (!isnan(value)) return value;
	}
	return NAN;
}

bool InputBindings::evaluateAction(const Iterator& in_iter) {
	return in_iter.mActionIt->second(in_iter.mEvent, in_iter.mState);
}

InputStateChange InputBindings::evaluateState(const Iterator& in_iter) {
	return in_iter.mStateIt->second(in_iter.mEvent, in_iter.mState);
}

float InputBindings::evaluateRange(const Iterator& in_iter) {
	return in_iter.mRangeIt->second(in_iter.mEvent, in_iter.mState);
}

InputBindings::Iterator InputBindings::iterateActions(const RawInputEvent& in_event, const RawInputState& in_state) {
	return Iterator(Iterator::Mode::action, *this, in_event, in_state);
}

InputBindings::Iterator InputBindings::iterateStates(const RawInputEvent& in_event, const RawInputState& in_state) {
	return Iterator(Iterator::Mode::state, *this, in_event, in_state);
}

InputBindings::Iterator InputBindings::iterateRanges(const RawInputEvent& in_event, const RawInputState& in_state) {
	return Iterator(Iterator::Mode::range, *this, in_event, in_state);
}

InputBindings::Iterator::Iterator(Mode in_mode, InputBindings& in_parent, const RawInputEvent& in_event, const RawInputState& in_state) :
	mMode(in_mode),
	mParent(in_parent),
	mEvent(in_event),
	mState(in_state) {

	switch (mMode) {
	case Mode::action:
		for (mActionIt = mParent.mActions.begin(); bool(*this) && !mActionIt->second(mEvent, mState); mActionIt++);
		break;
	case Mode::state:
		for (mStateIt = mParent.mStates.begin(); bool(*this) && mStateIt->second(mEvent, mState) == InputStateChange::no_change; mStateIt++);
		break;
	case Mode::range:
		for (mRangeIt = mParent.mRanges.begin(); bool(*this) && std::isnan(mRangeIt->second(mEvent, mState));  mRangeIt++);
		break;
	}
}

InputBindings::Iterator::operator bool() {
	switch (mMode) {
	case Mode::action:
		return mActionIt != mParent.mActions.end();
		break;
	case Mode::state:
		return mStateIt != mParent.mStates.end();
		break;
	case Mode::range:
		return mRangeIt != mParent.mRanges.end();
		break;
	}
	return false;
}

InputBindings::Iterator& InputBindings::Iterator::operator++() {
	switch (mMode) {
	case Mode::action:
		do { mActionIt++; } while (bool(*this) && !mActionIt->second(mEvent, mState));
		break;
	case Mode::state:
		do { mStateIt++; } while (bool(*this) && mStateIt->second(mEvent, mState) == InputStateChange::no_change);
		break;
	case Mode::range:
		do { mRangeIt++; } while (bool(*this) && std::isnan(mRangeIt->second(mEvent, mState)));
		break;
	}
	return *this;
}

InputBindings::key_t InputBindings::operator[](const Iterator& in_iterator) const {
	switch (in_iterator.mMode) {
	case Iterator::Mode::action:
		return in_iterator.mActionIt->first;
		break;
	case Iterator::Mode::state:
		return in_iterator.mStateIt->first;
		break;
	case Iterator::Mode::range:
		return in_iterator.mRangeIt->first;
		break;
	}
	return 0;
}

