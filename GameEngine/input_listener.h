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
		bool taken;
		bool eaten;
	};
private:
	InputContext* context;
	std::vector<InputEvent> eventQueue;

	uint level;
	double depth;

	std::function<Reception(const InputEvent&)> receiver;

public:
	InputListener(InputContext* in_context, uint in_level = 0, double in_depth = 0.0);

	InputContext* Get_Context();

	bool Post_Event(const InputEvent& in_event);
	InputEvent Pop_Event();
	uint Get_Number_Events();
	void Clear_Events();

	uint Get_Level() const;
	void Set_Level(uint in_level);

	double Get_Depth() const;
	void Set_Depth(double in_depth);

	void Set_Receiver(const std::function<Reception(const InputEvent&)>& in_receiver);

	static bool Compare(const InputListener& in_listener1, const InputListener& in_listener2);
	static bool Compare_Pointers(const InputListener* in_listener1, const InputListener* in_listener2);
};

#endif