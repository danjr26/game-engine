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
	sprite->Get_Transform().Set_Parent(&rigidBody->Get_Transform());
	GE.Physics().Add(rigidBody);
	rigidBody->Get_Transform().Set_Local_Position(Vector2d(Random<double>(0, 800), Random<double>(0, 600)));
	rigidBody->Set_Angular_Velocity(URotation2d(PI / 2));
	//rigidBody->Set_Linear_Velocity((Vector2d(400, 600) - rigidBody->Get_Transform().Get_World_Position()).Normalized() * 80.0);
}

void TestSpriteMover::Update(double in_dt) 
{}

TestSpriteMover2::TestSpriteMover2(Sprite* in_sprite) :
	sprite(in_sprite),
	accum(0.0) {
	
	transitioner.keys.resize(3);

	transitioner.Fill_Uniform_Durations(5.0);

	transitioner.keys[0].value.position = Vector2d(0, 0);
	transitioner.keys[1].value.position = Vector2d(600, 300);
	transitioner.keys[2].value.position = Vector2d(400, 600);

	transitioner.Fill_Basic_Tangents();

	/*
	transitioner.keys[0].value.vector.Normalize();
	transitioner.keys[0].value.vector *= 200.0;

	transitioner.keys[1].value.vector.Normalize();
	transitioner.keys[1].value.vector *= 200.0;

	transitioner.keys[2].value.vector.Normalize();
	transitioner.keys[2].value.vector *= 200.0;
	*/
	//transitioner.keys[0].value.vector = Vector2d(-1000, 0);
	//transitioner.keys[2].value.vector = Vector2d(500, 0);

	//transitioner.keys[0].duration = 8.0;
}

void TestSpriteMover2::Update(double in_dt) {
	accum += in_dt;

	LocatedVector2d pos = transitioner.Evaluate(accum);

	sprite->Get_Transform().Set_Local_Position(pos.position);
	sprite->Get_Transform().Set_Local_Rotation(Rotation2d(pos.vector));
}
