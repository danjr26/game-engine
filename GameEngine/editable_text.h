#ifndef EDITABLE_TEXT_H
#define EDITABLE_TEXT_H

#include "text2.h"
#include "input_listener.h"
#include "per_frame_updateable_object.h"
#include "text_input_context.h"
#include "sprite.h"
#include "stepper.h"

class EditableText : TransformableObject, PerFrameUpdateableObject {
private:
	InputListener input;
	Text2 text;
	Sprite cursorSprite;
	Stepper cursorBlinkStepper;
	uint cursorPos;

public:
	EditableText(TextInputContext* in_context, FontFaceRasterSet* in_rasterSet, Transform transform);
	void Set_Cursor_Position(uint in_cursorPos);
	void Increment_Cursor();
	void Decrement_Cursor();
	void Lower_Cursor();
	void Raise_Cursor();
	void Update(double in_dt) override;
};

#endif