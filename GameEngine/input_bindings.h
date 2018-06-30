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
		Mode mode;
		InputBindings& parent;
		const RawInputEvent& _event;
		const RawInputState& state;
		int index;

	private:
		Iterator(Mode in_mode, InputBindings& in_parent, const RawInputEvent& in_event, const RawInputState& in_state);
	public:
		operator bool();
		Iterator& operator++();
	};

public:
	InputBindings(uint in_nActions, uint in_nStates, uint in_nRanges);
	uint Get_Number_Actions() const;
	uint Get_Number_States() const;
	uint Get_Number_Ranges() const;
	void Bind_Action(uint in_slot, InputActionIdentifier in_action);
	void Bind_State(uint in_slot, InputStateIdentifier in_state);
	void Bind_Range(uint in_slot, InputRangeIdentifier in_range);
	void Clear_Action(uint in_slot);
	void Clear_State(uint in_slot);
	void Clear_Range(uint in_slot);
	InputActionIdentifier Get_Bound_Action(uint in_slot);
	InputStateIdentifier Get_Bound_State(uint in_slot);
	InputRangeIdentifier Get_Bound_Range(uint in_slot);
	bool Evaluate_Action(uint in_slot, const RawInputEvent& in_event, const RawInputState& in_state);
	InputStateChange Evaluate_State(uint in_slot, const RawInputEvent& in_event, const RawInputState& in_state);
	float Evaluate_Range(uint in_slot, const RawInputEvent& in_event, const RawInputState& in_state);
	bool Evaluate_Action(const Iterator& in_iter);
	InputStateChange Evaluate_State(const Iterator& in_iter);
	float Evaluate_Range(const Iterator& in_iter);
	Iterator Iterate_Actions(const RawInputEvent& in_event, const RawInputState& in_state);
	Iterator Iterate_States(const RawInputEvent& in_event, const RawInputState& in_state);
	Iterator Iterate_Ranges(const RawInputEvent& in_event, const RawInputState& in_state);
	uint operator[](Iterator in_iterator) const;
};

#endif