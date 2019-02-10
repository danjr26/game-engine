#ifndef POINTER_INPUT_CONTEXT_H
#define POINTER_INPUT_CONTEXT_H

#include "input_context.h"

class PointerInputContext : public InputContext {
public:
	struct Action {
		enum {
			select1,
			select2,
			select3,
			count
		};
	};

	struct State {
		enum {
			select1_down,
			select2_down,
			select3_down,
			count
		};
	};

	struct Range {
		enum {
			pixel_x,
			pixel_y,
			count
		};
	};

	PointerInputContext();
	bool processRawEvent(const RawInputEvent& in_event) override;
};


#endif