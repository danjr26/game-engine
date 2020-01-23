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
		//union {
		actions_it_t mActionIt;
		states_it_t mStateIt;
		ranges_it_t mRangeIt;
		//};

	private:
		Iterator(Mode i_mode, InputBindings& i_parent, const RawInputEvent& i_event, const RawInputState& i_state);
	public:
		~Iterator();
		operator bool();
		Iterator& operator++();
	};

public:
	InputBindings();

	uint getNumberActions() const;
	uint getNumberStates() const;
	uint getNumberRanges() const;

	void bindAction(key_t i_slot, action_t i_action);
	void bindState(key_t i_slot, state_t i_state);
	void bindRange(key_t i_slot, range_t i_range);

	void clearAction(key_t i_slot);
	void clearState(key_t i_slot);
	void clearRange(key_t i_slot);

	bool evaluateAction(key_t i_slot, const RawInputEvent& i_event, const RawInputState& i_state);
	InputStateChange evaluateState(key_t i_slot, const RawInputEvent& i_event, const RawInputState& i_state);
	float evaluateRange(key_t i_slot, const RawInputEvent& i_event, const RawInputState& i_state);

	bool evaluateAction(const Iterator& i_iter);
	InputStateChange evaluateState(const Iterator& i_iter);
	float evaluateRange(const Iterator& i_iter);

	Iterator iterateActions(const RawInputEvent& i_event, const RawInputState& i_state);
	Iterator iterateStates(const RawInputEvent& i_event, const RawInputState& i_state);
	Iterator iterateRanges(const RawInputEvent& i_event, const RawInputState& i_state);

	key_t operator[](const Iterator& i_iterator) const;
};

#endif