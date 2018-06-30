#ifndef RAW_INPUT_EVENT_H
#define RAW_INPUT_EVENT_H

#include "key.h"
#include "mouse_button.h"
#include "vector.h"
#include "window.h"

struct RawInputEvent {
	HWND windowHandle;

	enum Type {
		mouse_down,
		mouse_up,
		mouse_wheel_x,
		mouse_wheel_y,
		mouse_move,
		key_down,
		key_up,
		character
	} type;

	union {
		struct RawMouseEvent {
			union {
				MouseButton button;
				float wheelValue;
			};
			Vector2i position;
		} mouse;

		struct RawKeyboardEvent {
			union {
				Key key;
				char character;
			};
			bool repeat;
		} keyboard;
	};

	RawInputEvent();
};

#endif