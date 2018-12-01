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
	rigidBody(nullptr) {

	rigidBody = new RigidBody2(in_mask);
	rigidBody->Get_Transform().Rotate_Local(Rotation2d(PI / 2));
	sprite->Get_Transform().Set_Parent(&rigidBody->Get_Transform());
	GE.Physics().Add(rigidBody);
	rigidBody->Get_Transform().Set_Local_Position(Vector2d(Random<double>(0, 800), Random<double>(0, 600)));
	rigidBody->Set_Linear_Velocity((Vector2d(400, 600) - rigidBody->Get_Transform().Get_World_Position()).Normalized() * 80.0);
}

void TestSpriteMover::Update(double in_dt) {
	
}
