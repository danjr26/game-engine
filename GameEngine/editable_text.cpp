#include "editable_text.h"
#include "game_engine.h"

EditableText::EditableText(TextInputContext* in_context, FontFaceRasterSet* in_rasterSet) :
	input(in_context),
	text("text", in_rasterSet),
	cursorSprite(AxisAlignedRectangled(
		AxisAlignedRectangled::From_Extrema(
			Vector2d(0, 0), 
			Vector2d(cursorWidth, text.Get_Char_Height())
		)
	), nullptr, ColorRGBAf(1, 1, 1, 1)),
	cursorBlinkStepper(0.5),
	cursorPos(0)
{
	text.Get_Transform().Set_Parent(&transform);
	text.Get_Depth_Transform().Set_Parent(&depthTransform);
	cursorSprite.Get_Transform().Set_Parent(&text.Get_Scroll_Transform());
	cursorSprite.Get_Depth_Transform().Set_Parent(&text.Get_Depth_Transform());
	Set_Cursor_Position(0);

	//text.Set_Scroll_Position(Vector2d(0, 50));
}

void EditableText::Set_Cursor_Position(uint in_cursorPos) {
	cursorPos = Min<uint>(in_cursorPos, (uint)text.Get_Text().size());
	Update_Cursor_Sprite();
	text.Set_Scroll_Position(text.Get_Scroll_To_Include_Char(cursorPos));
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
	newPos[1] += text.Get_Newline_Height() * 1.5f;
	Set_Cursor_Position(text.Get_Closest_Char_Index(newPos));
}

void EditableText::Raise_Cursor() {
	Vector2f newPos = text.Get_Local_Char_Position(cursorPos);
	newPos[1] -= text.Get_Newline_Height() * 0.5f;
	Set_Cursor_Position(text.Get_Closest_Char_Index(newPos));
}

void EditableText::Update(double in_dt) {
	InputEvent _event;
	while (input.Get_Number_Events()) {
		_event = input.Pop_Event();
		if (_event.type == InputEvent::Type::action) {
			switch (_event.message) {
			case TextInputContext::Action::backspace:
				if (cursorPos > 0) {
					text.Delete(cursorPos - 1, 1);
					Decrement_Cursor();
				}
				break;
			case TextInputContext::Action::_delete:
				if (cursorPos < text.Get_Text().size()) {
					text.Delete(cursorPos, 1);
				}
				break;
			case TextInputContext::Action::newline:
				text.Insert("\n", cursorPos);
				Increment_Cursor();
				break;
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
				Set_Cursor_Position((uint)text.Get_Text().size());
				break;
			default:
				text.Insert((char)(_event.message + ' '), cursorPos);
				Increment_Cursor();
				break;
			}
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

void EditableText::Update_Cursor_Sprite() {
	Vector2f position = text.Get_Local_Char_Position(cursorPos);

	if (position.X() > 0.0) {
		position[0] -= (float)(cursorSprite.Get_Transform().Get_Local_Scale().X() / 2.0);
	}

	cursorSprite.Get_Transform().Set_Local_Position(position);
}

double EditableText::Z() const {
	return depthTransform.Get_World_Depth();
}

bool EditableText::Should_Cull() const {
	return false;
}

void EditableText::Render() {
	Vector2f corner = Vector2f(Vector2d(transform.Get_Local_Position()));
	Vector2f dimensions = text.Get_Container_Dimensions();
	corner[1] = GE.Render().mainWindow->Get_Dimensions().Y() - corner.Y() - dimensions.Y();

	glScissor((int)corner.X(), (int)corner.Y(), (int)dimensions.X(), (int)dimensions.Y());
	glEnable(GL_SCISSOR_TEST);

	text.Render();
	if (cursorSprite.Is_Enabled()) {
		cursorSprite.Render();
	}
	glDisable(GL_SCISSOR_TEST);
}
