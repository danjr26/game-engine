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
	InputListener(InputContext* i_context, uint i_level = 0, double i_depth = 0.0);
	~InputListener();

	InputContext* getContext();

	bool postEvent(const InputEvent& i_event);
	InputEvent popEvent();
	uint getNumberEvents();
	void clearEvents();

	uint getLevel() const;
	void setLevel(uint i_level);

	double getDepth() const;
	void setDepth(double i_depth);

	void setReceiver(const std::function<Reception(const InputEvent&)>& i_receiver);

	static bool compare(const InputListener& i_listener1, const InputListener& i_listener2);
	static bool comparePointers(const InputListener* i_listener1, const InputListener* i_listener2);
};

#endif