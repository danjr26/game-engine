#ifndef INPUT_ACTION_BINDING_H
#define INPUT_ACTION_BINDING_H

#include <map>
#include "input_identifiers.h"

class InputBindings {
public:
	using action_t = InputActionIdentifier;
	using state_t = InputStateIdentifier;
	using range_t = InputRangeIdentifier;
	using key_t = uint;
	using actions_t = std::multimap<key_t, action_t>;
	using states_t = std::multimap<key_t, state_t>;
	using ranges_t = std::multimap<key_t, range_t>;
	using actions_it_t = typename actions_t::iterator;
	using states_it_t = typename states_t::iterator;
	using ranges_it_t = typename ranges_t::iterator;

private:
	actions_t mActions;
	states_t mStates;
	ranges_t mRanges;

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
		union {
			actions_it_t mActionIt;
			states_it_t mStateIt;
			ranges_it_t mRangeIt;
		};

	private:
		Iterator(Mode in_mode, InputBindings& in_parent, const RawInputEvent& in_event, const RawInputState& in_state);
	public:
		operator bool();
		Iterator& operator++();
	};

public:
	InputBindings();

	uint getNumberActions() const;
	uint getNumberStates() const;
	uint getNumberRanges() const;

	void bindAction(key_t in_slot, action_t in_action);
	void bindState(key_t in_slot, state_t in_state);
	void bindRange(key_t in_slot, range_t in_range);

	void clearAction(key_t in_slot);
	void clearState(key_t in_slot);
	void clearRange(key_t in_slot);

	bool evaluateAction(key_t in_slot, const RawInputEvent& in_event, const RawInputState& in_state);
	InputStateChange evaluateState(key_t in_slot, const RawInputEvent& in_event, const RawInputState& in_state);
	float evaluateRange(key_t in_slot, const RawInputEvent& in_event, const RawInputState& in_state);

	bool evaluateAction(const Iterator& in_iter);
	InputStateChange evaluateState(const Iterator& in_iter);
	float evaluateRange(const Iterator& in_iter);

	Iterator iterateActions(const RawInputEvent& in_event, const RawInputState& in_state);
	Iterator iterateStates(const RawInputEvent& in_event, const RawInputState& in_state);
	Iterator iterateRanges(const RawInputEvent& in_event, const RawInputState& in_state);

	key_t operator[](const Iterator& in_iterator) const;
};

#endif