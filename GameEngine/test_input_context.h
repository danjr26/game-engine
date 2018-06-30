#ifndef TEST_INPUT_CONTEXT_H
#define TEST_INPUT_CONTEXT_H

#include "per_frame_updateable_object.h"
#include "input_listener.h"
#include "input_context.h"
#include "sprite.h"
#include "keyboard_action_identifier.h"
#include "keyboard_range_identifier.h"
#include "mouse_range_identifier.h"

class TestInputContext : public InputContext {
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
			movement_x,
			movement_y,
			count
		};
	};

	TestInputContext();
	bool Process_Raw_Event(const RawInputEvent& in_event) override;
};

class TestSpriteMover : public PerFrameUpdateableObject {
private:
	InputListener input;
	TransformableObject* object;

public:
	TestSpriteMover(TestInputContext* in_inputContext, TransformableObject* in_object);
	void Update(double in_dt) override;
};

#endif
