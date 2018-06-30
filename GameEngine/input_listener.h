#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include <vector>
#include <memory>
#include "log.h"
#include "input_event.h"

class InputContext;

class InputListener {
private:
	InputContext* context;
	std::vector<InputEvent> eventQueue;

public:
	InputListener(InputContext* in_context);
	void Post_Event(InputEvent in_event);
	InputEvent Pop_Event();
	uint Get_Number_Events();
	void Clear_Events();
	InputContext* Get_Context();
};

#endif