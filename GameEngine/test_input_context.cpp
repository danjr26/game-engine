#include "test_input_context.h"
#include "game_engine.h"

TestInputContext::TestInputContext() {

	mBindings.bindRange(Range::pointer_x, MouseRangeIdentifier(MouseRangeIdentifier::MouseAxis::x));
	mBindings.bindRange(Range::pointer_y, MouseRangeIdentifier(MouseRangeIdentifier::MouseAxis::y));
	mBindings.bindRange(Range::movement_x, KeyboardRangeIdentifier(Key::a, Key::d));
	mBindings.bindRange(Range::movement_y, KeyboardRangeIdentifier(Key::w, Key::s));
}

bool TestInputContext::processRawEvent(const RawInputEvent& in_event) {
	if (in_event.mType == RawInputEvent::Type::key_down && in_event.mKeyboard.mKey == Key::escape) GE.quit();
	return autoUpdate(in_event);
}

TestSpriteMover::TestSpriteMover(Sprite* in_sprite, CollisionMask2d& in_mask) :
	mSprite(in_sprite),
	mRigidBody(nullptr) {

	mRigidBody = new RigidBody2(in_mask);
	mSprite->getTransform().setParent(&mRigidBody->getTransform());
	GE.physics().add(mRigidBody);
	mRigidBody->getTransform().setLocalPosition(Vector2d(GEUtil::random<double>(0, 800), GEUtil::random<double>(0, 600)));
	mRigidBody->setAngularVelocity(URotation2d(PI / 2));
	//rigidBody->setLinearVelocity((Vector2d(400, 600) - rigidBody->getTransform().getWorldPosition()).normalized() * 80.0);
}

void TestSpriteMover::update(double in_dt) 
{}

TestSpriteMover2::TestSpriteMover2(Sprite* in_sprite) :
	mSprite(in_sprite),
	mAccum(0.0) {
	
	mTransitioner.mKeys.resize(3);

	mTransitioner.fillUniformDurations(5.0);

	mTransitioner.mKeys[0].mValue.mPosition = Vector2d(300, 0);
	mTransitioner.mKeys[1].mValue.mPosition = Vector2d(600, 300);
	mTransitioner.mKeys[2].mValue.mPosition = Vector2d(300, 600);

	mTransitioner.fillBasicTangents();

	/*
	transitioner.mKeys[0].mValue.vector.normalize();
	transitioner.mKeys[0].mValue.vector *= 200.0;

	transitioner.mKeys[1].mValue.vector.normalize();
	transitioner.mKeys[1].mValue.vector *= 200.0;

	transitioner.mKeys[2].mValue.vector.normalize();
	transitioner.mKeys[2].mValue.vector *= 200.0;
	*/
	//transitioner.mKeys[0].mValue.vector = Vector2d(-1000, 0);
	//transitioner.mKeys[2].mValue.vector = Vector2d(500, 0);

	//transitioner.mKeys[0].duration = 8.0;
}

void TestSpriteMover2::update(double in_dt) {
	mAccum += in_dt;

	LocatedVector2d pos = mTransitioner.evaluate(mAccum);

	mSprite->getTransform().setLocalPosition(pos.mPosition);
	mSprite->getTransform().setLocalRotation(Rotation2d(pos.mVector));
}
