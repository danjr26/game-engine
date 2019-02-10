#include "test_input_context.h"
#include "game_engine.h"

TestInputContext::TestInputContext() :
	InputContext(Action::count, State::count, Range::count) {

	mBindings.Bind_Range(Range::pointer_x, MouseRangeIdentifier(MouseRangeIdentifier::MouseAxis::x));
	mBindings.Bind_Range(Range::pointer_y, MouseRangeIdentifier(MouseRangeIdentifier::MouseAxis::y));
	mBindings.Bind_Range(Range::movement_x, KeyboardRangeIdentifier(Key::a, Key::d));
	mBindings.Bind_Range(Range::movement_y, KeyboardRangeIdentifier(Key::w, Key::s));
}

bool TestInputContext::Process_Raw_Event(const RawInputEvent& in_event) {
	if (in_event.mType == RawInputEvent::Type::key_down && in_event.mKeyboard.mKey == Key::escape) GE.Exit();
	return Auto_Update(in_event);
}

TestSpriteMover::TestSpriteMover(Sprite* in_sprite, CollisionMask2d& in_mask) :
	mSprite(in_sprite),
	mRigidBody(nullptr) {

	mRigidBody = new RigidBody2(in_mask);
	mSprite->Get_Transform().Set_Parent(&mRigidBody->Get_Transform());
	GE.Physics().Add(mRigidBody);
	mRigidBody->Get_Transform().Set_Local_Position(Vector2d(Random<double>(0, 800), Random<double>(0, 600)));
	mRigidBody->Set_Angular_Velocity(URotation2d(PI / 2));
	//rigidBody->Set_Linear_Velocity((Vector2d(400, 600) - rigidBody->Get_Transform().Get_World_Position()).Normalized() * 80.0);
}

void TestSpriteMover::Update(double in_dt) 
{}

TestSpriteMover2::TestSpriteMover2(Sprite* in_sprite) :
	mSprite(in_sprite),
	mAccum(0.0) {
	
	mTransitioner.mKeys.resize(3);

	mTransitioner.Fill_Uniform_Durations(5.0);

	mTransitioner.mKeys[0].mValue.mPosition = Vector2d(300, 0);
	mTransitioner.mKeys[1].mValue.mPosition = Vector2d(600, 300);
	mTransitioner.mKeys[2].mValue.mPosition = Vector2d(300, 600);

	mTransitioner.Fill_Basic_Tangents();

	/*
	transitioner.mKeys[0].mValue.vector.Normalize();
	transitioner.mKeys[0].mValue.vector *= 200.0;

	transitioner.mKeys[1].mValue.vector.Normalize();
	transitioner.mKeys[1].mValue.vector *= 200.0;

	transitioner.mKeys[2].mValue.vector.Normalize();
	transitioner.mKeys[2].mValue.vector *= 200.0;
	*/
	//transitioner.mKeys[0].mValue.vector = Vector2d(-1000, 0);
	//transitioner.mKeys[2].mValue.vector = Vector2d(500, 0);

	//transitioner.mKeys[0].duration = 8.0;
}

void TestSpriteMover2::Update(double in_dt) {
	mAccum += in_dt;

	LocatedVector2d pos = mTransitioner.Evaluate(mAccum);

	mSprite->Get_Transform().Set_Local_Position(pos.mPosition);
	mSprite->Get_Transform().Set_Local_Rotation(Rotation2d(pos.mVector));
}
