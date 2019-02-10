#ifndef INPUT_CONTEXT_H
#define INPUT_CONTEXT_H

#include "raw_input_state.h"
#include "input_event.h"
#include "input_listener.h"
#include "input_bindings.h"


class InputContext {
protected:
	InputBindings mBindings;

	std::vector<bool> mStates;
	std::vector<float> mRanges;

	std::vector<InputListener*> mListeners;

protected:
	InputContext(uint in_nActions, uint in_nStates, uint in_nRanges);

	InputEvent autoTranslateAction(InputBindings::Iterator& in_iter);
	InputEvent autoTranslateState(InputBindings::Iterator& in_iter, bool in_newValue);
	InputEvent autoTranslateRange(InputBindings::Iterator& in_iter, float in_newValue);

	bool autoProcessAction(InputBindings::Iterator& in_iter);
	bool autoProcessState(InputBindings::Iterator& in_iter);
	bool autoProcessRange(InputBindings::Iterator& in_iter);

	bool autoUpdateActions(const RawInputEvent& in_event);
	bool autoUpdateStates(const RawInputEvent& in_event);
	bool autoUpdateRanges(const RawInputEvent& in_event);

	bool autoUpdate(const RawInputEvent& in_event);

	virtual bool distributeEvent(const InputEvent& in_event);

public:
	virtual bool processRawEvent(const RawInputEvent& in_event);

	bool getState(uint in_index);
	float getRange(uint in_index);

	void add(InputListener* in_listener);
	void remove(InputListener* in_listener);
	void sort();
};

#endif