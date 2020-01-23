#ifndef INPUT_CONTEXT_H
#define INPUT_CONTEXT_H

#include "raw_input_state.h"
#include "input_event.h"
#include "input_listener.h"
#include "input_bindings.h"


class InputContext {
protected:
	InputBindings mBindings;

	std::map<uint, bool> mStates;
	std::map<uint, float> mRanges;

	std::vector<InputListener*> mListeners;

	float mPriority;

protected:
	InputContext();

	InputEvent autoTranslateAction(InputBindings::Iterator& i_iter);
	InputEvent autoTranslateState(InputBindings::Iterator& i_iter, bool i_newValue);
	InputEvent autoTranslateRange(InputBindings::Iterator& i_iter, float i_newValue);

	bool autoProcessAction(InputBindings::Iterator& i_iter);
	bool autoProcessState(InputBindings::Iterator& i_iter);
	bool autoProcessRange(InputBindings::Iterator& i_iter);

	bool autoUpdateActions(const RawInputEvent& i_event);
	bool autoUpdateStates(const RawInputEvent& i_event);
	bool autoUpdateRanges(const RawInputEvent& i_event);

	bool autoUpdate(const RawInputEvent& i_event);

	virtual bool distributeEvent(const InputEvent& i_event);

public:
	virtual bool processRawEvent(const RawInputEvent& i_event);

	bool getState(uint i_index);
	float getRange(uint i_index);

	float getPriority() const;
	void setPriority(float i_priority);

	void add(InputListener* i_listener);
	void remove(InputListener* i_listener);
	void sort();
};

#endif