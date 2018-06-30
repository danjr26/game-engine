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

TestSpriteMover::TestSpriteMover(TestInputContext* in_inputContext, TransformableObject* in_object) :
	input(in_inputContext),
	object(in_object)
{}

void TestSpriteMover::Update(double in_dt) {
	InputEvent _event;
	while (input.Get_Number_Events()) {
		_event = input.Pop_Event();
		/*
		switch (_event.type) {
		case TestInputContext::Action::start_move_up:
			object->Get_Transform().Translate(Vector3d(0, -5, 0));
			break;
		case TestInputContext::Action::start_move_left:
			object->Get_Transform().Translate(Vector3d(-5, 0, 0));
			break;
		case TestInputContext::Action::start_move_down:
			object->Get_Transform().Translate(Vector3d(0, 5, 0));
			break;
		case TestInputContext::Action::start_move_right:
			object->Get_Transform().Translate(Vector3d(5, 0, 0));
			break;
		}
		*/
	}
	/*object->Get_Transform().Translate(Vector3f(
		input.Get_Context()->Get_Range(TestInputContext::Range::movement_x),
		input.Get_Context()->Get_Range(TestInputContext::Range::movement_y),
		0.0) * 100.0f * in_dt);*/

	object->Get_Transform().Set_Position(
		DisplayUnits3::Percent(
			Vector3f(
				input.Get_Context()->Get_Range(TestInputContext::Range::pointer_x),
				input.Get_Context()->Get_Range(TestInputContext::Range::pointer_y),
				0.0
			) * 100.0f
		).Pixels(GE.Render().mainWindow->Get_Dimensions())
	);
}
