#ifndef TEST_INPUT_CONTEXT_H
#define TEST_INPUT_CONTEXT_H

#include "per_frame_updateable_object.h"
#include "input_listener.h"
#include "input_context.h"
#include "sprite.h"
#include "keyboard_action_identifier.h"
#include "keyboard_range_identifier.h"
#include "mouse_range_identifier.h"
//#include "rectangle_collision_mask.h"
#include "collision_context.h"
#include "rigid_body.h"
#include "hermite_transitioner.h"
#include "catmull_rom_transitioner.h"
#include "basic_collision_mask.h"

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
	bool processRawEvent(const RawInputEvent& in_event) override;
};

class TestSpriteMover : public PerFrameUpdateableObject {
private:
	Sprite* mSprite;
	RigidBody2* mRigidBody;

public:
	TestSpriteMover(Sprite* in_sprite, CollisionMask2d& in_mask);
	void update(double in_dt) override;
};

class TestSpriteMover2 : public PerFrameUpdateableObject {
private:
	Sprite* mSprite;
	double mAccum;
	HermiteTransitioner<double, 2> mTransitioner;

public:
	TestSpriteMover2(Sprite* in_sprite);
	void update(double in_dt) override;
};

#endif
