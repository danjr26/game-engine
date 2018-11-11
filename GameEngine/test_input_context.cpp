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

TestSpriteMover::TestSpriteMover(TestInputContext* in_inputContext, TransformableObject2f* in_object, CollisionMask2d* in_collisionMask) :
	input(in_inputContext),
	object(in_object),
	collisionMask(in_collisionMask) ,
	collisionContext(GE.Collision().Get_Active2())
{
	
}

void TestSpriteMover::Update(double in_dt) {
	/*object->Get_Transform().Translate_Local(Vector3f(
		input.Get_Context()->Get_Range(TestInputContext::Range::movement_x),
		input.Get_Context()->Get_Range(TestInputContext::Range::movement_y),
		0.0) * 100.0f * in_dt);
	*/
	
	object->Get_Transform().Set_Local_Position(
		DisplayUnits2f::Percent(
			Vector2f(
				input.Get_Context()->Get_Range(TestInputContext::Range::pointer_x),
				input.Get_Context()->Get_Range(TestInputContext::Range::pointer_y)
			) * 100.0f
		).Pixels(GE.Render().mainWindow->Get_Dimensions())
	);
	
	collisionMask->Get_Transform().Set_Local_Position(object->Get_Transform().Get_Local_Position() + Vector2d(50, 50));
	auto partners = collisionContext->Get_Partners(collisionMask);
	if (partners.first == partners.second) {
		
	}
	else {
		Log::main(std::string("collision ") + to_string(time(0)));
	}
}
