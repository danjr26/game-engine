#ifndef OVERLAY_INPUT_CONTEXT_H
#define OVERLAY_INPUT_CONTEXT_H

#include "input_context.h"

class OverlayInputContext : public InputContext {
public:
	struct Action {
		enum : uint {
			count
		};
	};

	struct State {
		enum : uint {
			count
		};
	};

	struct Range {
		enum : uint {
			pointer_x,
			pointer_y,
			count
		};
	};

	OverlayInputContext();
	bool Process_Raw_Event(const RawInputEvent& in_event) override;
};

#endif