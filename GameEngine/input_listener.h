#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include <vector>
#include <memory>
#include "definitions.h"
#include "log.h"
#include "input_event.h"

class InputContext;

class InputListener {
public:
	struct Reception {
		bool mTaken;
		bool mEaten;
	};
private:
	InputContext* mContext;
	std::vector<InputEvent> mEventQueue;

	uint mLevel;
	double mDepth;

	std::function<Reception(const InputEvent&)> mReceiver;

public:
	InputListener(InputContext* in_context, uint in_level = 0, double in_depth = 0.0);
	~InputListener();

	InputContext* getContext();

	bool postEvent(const InputEvent& in_event);
	InputEvent popEvent();
	uint getNumberEvents();
	void clearEvents();

	uint getLevel() const;
	void setLevel(uint in_level);

	double getDepth() const;
	void setDepth(double in_depth);

	void setReceiver(const std::function<Reception(const InputEvent&)>& in_receiver);

	static bool compare(const InputListener& in_listener1, const InputListener& in_listener2);
	static bool comparePointers(const InputListener* in_listener1, const InputListener* in_listener2);
};

#endif