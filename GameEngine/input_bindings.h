#ifndef INPUT_ACTION_BINDING_H
#define INPUT_ACTION_BINDING_H

#include <vector>
#include "input_identifiers.h"

class InputBindings {
private:
	std::vector<InputActionIdentifier> actions;
	std::vector<InputStateIdentifier> states;
	std::vector<InputRangeIdentifier> ranges;

public:
	class Iterator {
		friend class InputBindings;
		enum class Mode {
			action,
			state,
			range
		};
		Mode mMode;
		InputBindings& mParent;
		const RawInputEvent& mEvent;
		const RawInputState& mState;
		int mIndex;

	private:
		Iterator(Mode in_mode, InputBindings& in_parent, const RawInputEvent& in_event, const RawInputState& in_state);
	public:
		operator bool();
		Iterator& operator++();
	};

public:
	InputBindings(uint in_nActions, uint in_nStates, uint in_nRanges);
	uint getNumberActions() const;
	uint getNumberStates() const;
	uint getNumberRanges() const;
	void bindAction(uint in_slot, InputActionIdentifier in_action);
	void bindState(uint in_slot, InputStateIdentifier in_state);
	void bindRange(uint in_slot, InputRangeIdentifier in_range);
	void clearAction(uint in_slot);
	void clearState(uint in_slot);
	void clearRange(uint in_slot);
	InputActionIdentifier getBoundAction(uint in_slot);
	InputStateIdentifier getBoundState(uint in_slot);
	InputRangeIdentifier getBoundRange(uint in_slot);
	bool evaluateAction(uint in_slot, const RawInputEvent& in_event, const RawInputState& in_state);
	InputStateChange evaluateState(uint in_slot, const RawInputEvent& in_event, const RawInputState& in_state);
	float evaluateRange(uint in_slot, const RawInputEvent& in_event, const RawInputState& in_state);
	bool evaluateAction(const Iterator& in_iter);
	InputStateChange evaluateState(const Iterator& in_iter);
	float evaluateRange(const Iterator& in_iter);
	Iterator iterateActions(const RawInputEvent& in_event, const RawInputState& in_state);
	Iterator iterateStates(const RawInputEvent& in_event, const RawInputState& in_state);
	Iterator iterateRanges(const RawInputEvent& in_event, const RawInputState& in_state);
	uint operator[](Iterator in_iterator) const;
};

#endif