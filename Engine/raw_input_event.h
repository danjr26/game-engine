#ifndef RAW_INPUT_EVENT_H
#define RAW_INPUT_EVENT_H

#include "key.h"
#include "mouse_button.h"
#include "vector.h"
#include "window.h"

struct RawInputEvent {
	HWND mWindowHandle;

	enum Type {
		mouse_down,
		mouse_up,
		mouse_wheel_x,
		mouse_wheel_y,
		mouse_move,
		key_down,
		key_up,
		character
	} mType;

	union {
		struct RawMouseEvent {
			union {
				MouseButton mButton;
				float mWheelValue;
			};
			Vector2i mPosition;
		} mMouse;

		struct RawKeyboardEvent {
			union {
				Key mKey;
				char mCharacter;
			};
			bool mRepeat;
		} mKeyboard;
	};

	RawInputEvent();
};

#endif