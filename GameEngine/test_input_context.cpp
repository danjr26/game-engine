#include "test_input_context.h"
#include "game_engine.h"

TestInputContext::TestInputContext() :
	InputContext(Action::count, State::count, Range::count) {

	bindings.Bind_Range(Range::pointer_x, MouseRangeIdentifier(MouseRangeIdentifier::MouseAxis::x));
	bindings.Bind_Range(Range::pointer_y, MouseRangeIdentifier(MouseRangeIdentifier::MouseAxis::y));
	bindings.Bind_Range(Range::movement_x, KeyboardRangeIdentifier(Key::a, Key::d));
	bindings.Bind_Range(Range::movement_y, KeyboardRangeIdentifier(Key::w, Key::s));
}

bool TestInputContext::Process_Raw_Event(const RawInputEvent& in_event) {
	return Auto_Update(in_event);
}

TestSpriteMover::TestSpriteMover(Sprite* in_sprite, CollisionMask2d& in_mask) :
	sprite(in_sprite),
	rigidBody(new RigidBody2(in_mask)) {

	sprite->Get_Transform().Set_Parent(&rigidBody->Get_Transform());
	GE.Physics().Add(rigidBody);
	rigidBody->Set_Linear_Velocity(Vector2d(30, 30));
}

void TestSpriteMover::Update(double in_dt) {
	
}
