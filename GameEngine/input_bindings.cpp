#include "input_bindings.h"
#include <cmath>

InputBindings::InputBindings(uint in_nActions, uint in_nStates, uint in_nRanges) :
	actions(in_nActions),
	states(in_nStates),
	ranges(in_nRanges) {
	
	for (uint i = 0; i < in_nActions; i++) { actions[i] = nullptr; }
	for (uint i = 0; i < in_nStates; i++) { states[i] = nullptr; }
	for (uint i = 0; i < in_nRanges; i++) { ranges[i] = nullptr; }
}

uint InputBindings::getNumberActions() const {
	return (uint)actions.size();
}

uint InputBindings::getNumberStates() const {
	return (uint)states.size();
}

uint InputBindings::getNumberRanges() const {
	return (uint)ranges.size();
}

void InputBindings::bindAction(uint in_slot, InputActionIdentifier in_action) {
	actions[in_slot] = in_action;
}

void InputBindings::bindState(uint in_slot, InputStateIdentifier in_state) {
	states[in_slot] = in_state;
}

void InputBindings::bindRange(uint in_slot, InputRangeIdentifier in_range) {
	ranges[in_slot] = in_range;
}

void InputBindings::clearAction(uint in_slot) {
	actions[in_slot] = nullptr;
}

void InputBindings::clearState(uint in_slot) {
	states[in_slot] = nullptr;
}

void InputBindings::clearRange(uint in_slot) {
	ranges[in_slot] = nullptr;
}

InputActionIdentifier InputBindings::getBoundAction(uint in_slot) {
	return actions[in_slot];
}

InputStateIdentifier InputBindings::getBoundState(uint in_slot) {
	return states[in_slot];
}

InputRangeIdentifier InputBindings::getBoundRange(uint in_slot) {
	return ranges[in_slot];
}

bool InputBindings::evaluateAction(uint in_slot, const RawInputEvent& in_event, const RawInputState& in_state) {
	if (actions[in_slot] == nullptr) {
		return false;
	}
	else {
		return actions[in_slot](in_event, in_state);
	}
}

InputStateChange InputBindings::evaluateState(uint in_slot, const RawInputEvent& in_event, const RawInputState& in_state) {
	if (states[in_slot] == nullptr) {
		return InputStateChange::no_change;
	}
	else {
		return states[in_slot](in_event, in_state);
	}
}

float InputBindings::evaluateRange(uint in_slot, const RawInputEvent& in_event, const RawInputState& in_state) {
	if (ranges[in_slot] == nullptr) {
		return NAN;
	}
	else {
		return ranges[in_slot](in_event, in_state);
	}
}

bool InputBindings::evaluateAction(const Iterator& in_iter) {
	return evaluateAction(in_iter.mIndex, in_iter.mEvent, in_iter.mState);
}

InputStateChange InputBindings::evaluateState(const Iterator& in_iter) {
	return evaluateState(in_iter.mIndex, in_iter.mEvent, in_iter.mState);
}

float InputBindings::evaluateRange(const Iterator& in_iter) {
	return evaluateRange(in_iter.mIndex, in_iter.mEvent, in_iter.mState);
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

uint InputBindings::operator[](Iterator in_iterator) const {
	return in_iterator.mIndex;
}

InputBindings::Iterator::Iterator(Mode in_mode, InputBindings& in_parent, const RawInputEvent& in_event, const RawInputState& in_state) :
	mMode(in_mode),
	mParent(in_parent),
	mEvent(in_event),
	mState(in_state),
	mIndex(-1) {
	
	++(*this);
}

InputBindings::Iterator::operator bool() {
	switch (mMode) {
	case Mode::action:
		return mIndex < (int)mParent.getNumberActions();
		break;
	case Mode::state:
		return mIndex < (int)mParent.getNumberStates();
		break;
	case Mode::range:
		return mIndex < (int)mParent.getNumberRanges();
		break;
	}
	return false;
}

InputBindings::Iterator& InputBindings::Iterator::operator++() {
	switch (mMode) {
	case Mode::action:
		do { mIndex++; } while (mIndex < (int)mParent.getNumberActions() && !mParent.evaluateAction(mIndex, mEvent, mState));
		break;
	case Mode::state:
		do { mIndex++; } while (mIndex < (int)mParent.getNumberStates() && mParent.evaluateState(mIndex, mEvent, mState) == InputStateChange::no_change);
		break;
	case Mode::range:
		do { mIndex++; } while (mIndex < (int)mParent.getNumberRanges() && std::isnan(mParent.evaluateRange(mIndex, mEvent, mState)));
		break;
	}
	return *this;
}


