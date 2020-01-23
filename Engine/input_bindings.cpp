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

void InputBindings::bindAction(key_t i_slot, action_t i_action) {
	mActions.insert(std::pair<uint, InputActionIdentifier>(i_slot, i_action));
}

void InputBindings::bindState(key_t i_slot, state_t i_state) {
	mStates.insert(std::pair<uint, InputStateIdentifier>(i_slot, i_state));
}

void InputBindings::bindRange(key_t i_slot, range_t i_range) {
	mRanges.insert(std::pair<uint, InputRangeIdentifier>(i_slot, i_range));
}

void InputBindings::clearAction(key_t i_slot) {
	mActions.erase(i_slot);
}

void InputBindings::clearState(key_t i_slot) {
	mStates.erase(i_slot);
}

void InputBindings::clearRange(key_t i_slot) {
	mRanges.erase(i_slot);
}

bool InputBindings::evaluateAction(key_t i_slot, const RawInputEvent& i_event, const RawInputState& i_state) {
	auto search = mActions.equal_range(i_slot);
	for (auto it = search.first; it != search.second; it++) {
		if (it->second(i_event, i_state)) return true;
	}
	return false;
}

InputStateChange InputBindings::evaluateState(key_t i_slot, const RawInputEvent& i_event, const RawInputState& i_state) {
	auto search = mStates.equal_range(i_slot);
	for (auto it = search.first; it != search.second; it++) {
		InputStateChange change = it->second(i_event, i_state);
		if (change != InputStateChange::no_change) return change;
	}
	return InputStateChange::no_change;
}

float InputBindings::evaluateRange(key_t i_slot, const RawInputEvent& i_event, const RawInputState& i_state) {
	auto search = mRanges.equal_range(i_slot);
	for (auto it = search.first; it != search.second; it++) {
		float value = it->second(i_event, i_state);
		if (!isnan(value)) return value;
	}
	return NAN;
}

bool InputBindings::evaluateAction(const Iterator& i_iter) {
	return i_iter.mActionIt->second(i_iter.mEvent, i_iter.mState);
}

InputStateChange InputBindings::evaluateState(const Iterator& i_iter) {
	return i_iter.mStateIt->second(i_iter.mEvent, i_iter.mState);
}

float InputBindings::evaluateRange(const Iterator& i_iter) {
	return i_iter.mRangeIt->second(i_iter.mEvent, i_iter.mState);
}

InputBindings::Iterator InputBindings::iterateActions(const RawInputEvent& i_event, const RawInputState& i_state) {
	return Iterator(Iterator::Mode::action, *this, i_event, i_state);
}

InputBindings::Iterator InputBindings::iterateStates(const RawInputEvent& i_event, const RawInputState& i_state) {
	return Iterator(Iterator::Mode::state, *this, i_event, i_state);
}

InputBindings::Iterator InputBindings::iterateRanges(const RawInputEvent& i_event, const RawInputState& i_state) {
	return Iterator(Iterator::Mode::range, *this, i_event, i_state);
}

InputBindings::Iterator::Iterator(Mode i_mode, InputBindings& i_parent, const RawInputEvent& i_event, const RawInputState& i_state) :
	mMode(i_mode),
	mParent(i_parent),
	mEvent(i_event),
	mState(i_state) {

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

InputBindings::Iterator::~Iterator() 
{}

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

InputBindings::key_t InputBindings::operator[](const Iterator& i_iterator) const {
	switch (i_iterator.mMode) {
	case Iterator::Mode::action:
		return i_iterator.mActionIt->first;
		break;
	case Iterator::Mode::state:
		return i_iterator.mStateIt->first;
		break;
	case Iterator::Mode::range:
		return i_iterator.mRangeIt->first;
		break;
	}
	return 0;
}

