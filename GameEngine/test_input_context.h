#ifndef TEST_INPUT_CONTEXT_H
#define TEST_INPUT_CONTEXT_H

#include "per_frame_updateable_object.h"
#include "input_listener.h"
#include "input_context.h"
#include "sprite.h"
#include "keyboard_action_identifier.h"
#include "keyboard_range_identifier.h"
#include "mouse_range_identifier.h"
#include "rectangle_collision_mask.h"
#include "collision_context.h"
#include "rigid_body.h"

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
	Sprite* sprite;
	RigidBody2* rigidBody;

public:
	TestSpriteMover(Sprite* in_sprite, CollisionMask2d& in_mask);
	void Update(double in_dt) override;
};

#endif
