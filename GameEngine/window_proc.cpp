#include "window_proc.h"
#include "game_engine.h"
#include "raw_input_event.h"
#include <windowsx.h>

LRESULT CALLBACK WindowProc(HWND in_hwnd, UINT in_message, WPARAM in_wParam, LPARAM in_lParam) {
	RawInputEvent inputEvent;
	ubyte xButton;
	switch (in_message) {
	case WM_DESTROY:
		GE.quit();
		return 0;
	case WM_LBUTTONDOWN:
		inputEvent.mWindowHandle = in_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_down;
		inputEvent.mMouse.mButton = MouseButton::left;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mMouse.mWheelValue = 0;
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_LBUTTONUP:
		inputEvent.mWindowHandle = in_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_up;
		inputEvent.mMouse.mButton = MouseButton::left;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mMouse.mWheelValue = 0;
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_MBUTTONDOWN:
		inputEvent.mWindowHandle = in_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_down;
		inputEvent.mMouse.mButton = MouseButton::middle;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mMouse.mWheelValue = 0;
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_MBUTTONUP:
		inputEvent.mWindowHandle = in_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_up;
		inputEvent.mMouse.mButton = MouseButton::middle;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mMouse.mWheelValue = 0;
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_RBUTTONDOWN:inputEvent.mWindowHandle = in_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_down;
		inputEvent.mMouse.mButton = MouseButton::right;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mMouse.mWheelValue = 0;
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_RBUTTONUP:
		inputEvent.mWindowHandle = in_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_up;
		inputEvent.mMouse.mButton = MouseButton::right;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mMouse.mWheelValue = 0;
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_XBUTTONDOWN:
		xButton = (ubyte)GET_XBUTTON_WPARAM(in_wParam);
		if (xButton == 1) {
			inputEvent.mWindowHandle = in_hwnd;
			inputEvent.mType = RawInputEvent::Type::mouse_down;
			inputEvent.mMouse.mButton = MouseButton::extra1;
			inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
			inputEvent.mMouse.mWheelValue = 0;
			GE.input().processRawEvent(inputEvent);
			return 0;
		}
		else if (xButton == 2) {
			inputEvent.mWindowHandle = in_hwnd;
			inputEvent.mType = RawInputEvent::Type::mouse_down;
			inputEvent.mMouse.mButton = MouseButton::extra2;
			inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
			inputEvent.mMouse.mWheelValue = 0;
			GE.input().processRawEvent(inputEvent);
			return 0;
		}
		return DefWindowProc(in_hwnd, in_message, in_wParam, in_lParam);
	case WM_XBUTTONUP:
		xButton = (ubyte)GET_XBUTTON_WPARAM(in_wParam);
		if (xButton == 1) {
			inputEvent.mWindowHandle = in_hwnd;
			inputEvent.mType = RawInputEvent::Type::mouse_up;
			inputEvent.mMouse.mButton = MouseButton::extra1;
			inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
			inputEvent.mMouse.mWheelValue = 0;
			GE.input().processRawEvent(inputEvent);
			return 0;
		}
		else if (xButton == 2) {
			inputEvent.mWindowHandle = in_hwnd;
			inputEvent.mType = RawInputEvent::Type::mouse_up;
			inputEvent.mMouse.mButton = MouseButton::extra2;
			inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
			inputEvent.mMouse.mWheelValue = 0;
			GE.input().processRawEvent(inputEvent);
			return 0;
		}
		return DefWindowProc(in_hwnd, in_message, in_wParam, in_lParam);
	case WM_MOUSEMOVE:
		inputEvent.mWindowHandle = in_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_move;
		inputEvent.mMouse.mButton = MouseButton::none;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mMouse.mWheelValue = 0;
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_MOUSEHWHEEL:
		inputEvent.mWindowHandle = in_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_wheel_x;
		inputEvent.mMouse.mButton = MouseButton::none;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mMouse.mWheelValue = (float)GET_WHEEL_DELTA_WPARAM(in_wParam) / WHEEL_DELTA;;
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_MOUSEWHEEL:
		inputEvent.mWindowHandle = in_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_wheel_y;
		inputEvent.mMouse.mButton = MouseButton::none;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mMouse.mWheelValue = (float)GET_WHEEL_DELTA_WPARAM(in_wParam) / WHEEL_DELTA;;
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_KEYDOWN:
		inputEvent.mWindowHandle = in_hwnd;
		inputEvent.mType = RawInputEvent::Type::key_down;
		inputEvent.mKeyboard.mKey = windowsVKToKey((uint)in_wParam);
		inputEvent.mKeyboard.mRepeat = in_lParam & (1 << 30);
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_KEYUP:
		inputEvent.mWindowHandle = in_hwnd;
		inputEvent.mType = RawInputEvent::Type::key_up;
		inputEvent.mKeyboard.mKey = windowsVKToKey((uint)in_wParam);
		inputEvent.mKeyboard.mRepeat = false;
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_CHAR:
		inputEvent.mWindowHandle = in_hwnd;
		inputEvent.mType = RawInputEvent::Type::character;
		inputEvent.mKeyboard.mCharacter = (char)in_wParam;
		inputEvent.mKeyboard.mRepeat = in_lParam & (1 << 30);
		GE.input().processRawEvent(inputEvent);
		return 0;
	default:
		return DefWindowProc(in_hwnd, in_message, in_wParam, in_lParam);
	}
}