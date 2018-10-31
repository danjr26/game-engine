#include "editable_text.h"
#include "game_engine.h"

EditableText::EditableText(TextInputContext* in_context, FontFaceRasterSet* in_rasterSet, Transform transform) :
	TransformableObject(transform),
	input(in_context),
	text("text", in_rasterSet, Transform()),
	cursorSprite(DeepAxisAlignedRectangled(
		AxisAlignedRectangled::From_Extrema(
			Vector2d(0, 0), 
			Vector2d(1, text.Get_Char_Height())
		), 
		transform.Get_Local_Position().Z()
	), nullptr, ColorRGBAf(1, 1, 1, 1)),
	cursorBlinkStepper(0.5),
	cursorPos(0)
{
	cursorSprite.Get_Transform().Set_Parent(&this->transform);
	GE.Render().Add(&text);
	GE.Render().Add(&cursorSprite);
}

void EditableText::Set_Cursor_Position(uint in_cursorPos) {
	cursorPos = Min<uint>(in_cursorPos, text.Get_Text().size());
	Vector3d newPos = Vector3d(text.Get_Local_Char_Position(cursorPos), 0.0);
	newPos[0] -= cursorSprite.Get_Transform().Get_Local_Scale().X() / 2.0;
	cursorSprite.Get_Transform().Set_Local_Position(newPos);
}

void EditableText::Increment_Cursor() {
	Set_Cursor_Position(cursorPos + 1);
}

void EditableText::Decrement_Cursor() {
	if (cursorPos > 0) {
		Set_Cursor_Position(cursorPos - 1);
	}
}

void EditableText::Lower_Cursor() {
	Vector2f newPos = text.Get_Local_Char_Position(cursorPos);
	newPos[1] += text.Get_Char_Height() * 1.5f;
	Set_Cursor_Position(text.Get_Closest_Char_Index(newPos));
}

void EditableText::Raise_Cursor() {
	Vector2f newPos = text.Get_Local_Char_Position(cursorPos);
	newPos[1] -= text.Get_Char_Height() * 0.5f;
	Set_Cursor_Position(text.Get_Closest_Char_Index(newPos));
}

void EditableText::Update(double in_dt) {
	InputEvent _event;
	while (input.Get_Number_Events()) {
		_event = input.Pop_Event();
		if (_event.type == InputEvent::Type::action) {
			std::string s = text.Get_Text();
			switch (_event.message) {
			case TextInputContext::Action::backspace:
				if (cursorPos > 0) {
					s.erase(s.begin() + cursorPos - 1);
					Decrement_Cursor();
				}
				break;
			case TextInputContext::Action::_delete:
				if (cursorPos < s.size()) {
					s.erase(s.begin() + cursorPos);
				}
				break;
			case TextInputContext::Action::newline:
				s.insert(s.begin() + cursorPos, '\n');
				Increment_Cursor();
			case TextInputContext::Action::cursor_left:
				Decrement_Cursor();
				break;
			case TextInputContext::Action::cursor_right:
				Increment_Cursor();
				break;
			case TextInputContext::Action::cursor_up:
				Raise_Cursor();
				break;
			case TextInputContext::Action::cursor_down:
				Lower_Cursor();
				break;
			case TextInputContext::Action::cursor_begin:
				Set_Cursor_Position(0);
				break;
			case TextInputContext::Action::cursor_end:
				Set_Cursor_Position(text.Get_Text().size());
				break;
			default:
				s.insert(s.begin() + cursorPos, _event.message + ' ');
				Increment_Cursor();

				break;
			}
			
			text.Replace(s);
		}
	}

	if (cursorBlinkStepper.Step_Total(in_dt)) {
		if (cursorSprite.Is_Enabled()) {
			cursorSprite.Disable();
		}
		else {
			cursorSprite.Enable();
		}
	}
}
