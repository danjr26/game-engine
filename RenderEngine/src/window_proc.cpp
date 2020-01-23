#include "window_proc.h"
#include "game_engine.h"
#include "raw_input_event.h"
#include <windowsx.h>

LRESULT CALLBACK WindowProc(HWND i_hwnd, UINT i_message, WPARAM i_wParam, LPARAM i_lParam) {
	RawInputEvent inputEvent;
	ubyte xButton;
	switch (i_message) {
	case WM_DESTROY:
		GE.quit();
		return 0;
	case WM_LBUTTONDOWN:
		inputEvent.mWindowHandle = i_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_down;
		inputEvent.mMouse.mButton = MouseButton::left;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(i_lParam), GET_Y_LPARAM(i_lParam));
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_LBUTTONUP:
		inputEvent.mWindowHandle = i_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_up;
		inputEvent.mMouse.mButton = MouseButton::left;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(i_lParam), GET_Y_LPARAM(i_lParam));
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_MBUTTONDOWN:
		inputEvent.mWindowHandle = i_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_down;
		inputEvent.mMouse.mButton = MouseButton::middle;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(i_lParam), GET_Y_LPARAM(i_lParam));
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_MBUTTONUP:
		inputEvent.mWindowHandle = i_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_up;
		inputEvent.mMouse.mButton = MouseButton::middle;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(i_lParam), GET_Y_LPARAM(i_lParam));
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_RBUTTONDOWN:
		inputEvent.mWindowHandle = i_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_down;
		inputEvent.mMouse.mButton = MouseButton::right;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(i_lParam), GET_Y_LPARAM(i_lParam));
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_RBUTTONUP:
		inputEvent.mWindowHandle = i_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_up;
		inputEvent.mMouse.mButton = MouseButton::right;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(i_lParam), GET_Y_LPARAM(i_lParam));
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_XBUTTONDOWN:
		xButton = (ubyte)GET_XBUTTON_WPARAM(i_wParam);
		if (xButton == 1) {
			inputEvent.mWindowHandle = i_hwnd;
			inputEvent.mType = RawInputEvent::Type::mouse_down;
			inputEvent.mMouse.mButton = MouseButton::extra1;
			inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(i_lParam), GET_Y_LPARAM(i_lParam));
			GE.input().processRawEvent(inputEvent);
			return 0;
		}
		else if (xButton == 2) {
			inputEvent.mWindowHandle = i_hwnd;
			inputEvent.mType = RawInputEvent::Type::mouse_down;
			inputEvent.mMouse.mButton = MouseButton::extra2;
			inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(i_lParam), GET_Y_LPARAM(i_lParam));
			GE.input().processRawEvent(inputEvent);
			return 0;
		}
		return DefWindowProc(i_hwnd, i_message, i_wParam, i_lParam);
	case WM_XBUTTONUP:
		xButton = (ubyte)GET_XBUTTON_WPARAM(i_wParam);
		if (xButton == 1) {
			inputEvent.mWindowHandle = i_hwnd;
			inputEvent.mType = RawInputEvent::Type::mouse_up;
			inputEvent.mMouse.mButton = MouseButton::extra1;
			inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(i_lParam), GET_Y_LPARAM(i_lParam));
			GE.input().processRawEvent(inputEvent);
			return 0;
		}
		else if (xButton == 2) {
			inputEvent.mWindowHandle = i_hwnd;
			inputEvent.mType = RawInputEvent::Type::mouse_up;
			inputEvent.mMouse.mButton = MouseButton::extra2;
			inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(i_lParam), GET_Y_LPARAM(i_lParam));
			GE.input().processRawEvent(inputEvent);
			return 0;
		}
		return DefWindowProc(i_hwnd, i_message, i_wParam, i_lParam);
	case WM_MOUSEMOVE:
		inputEvent.mWindowHandle = i_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_move;
		inputEvent.mMouse.mButton = MouseButton::none;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(i_lParam), GET_Y_LPARAM(i_lParam));
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_MOUSEHWHEEL:
		inputEvent.mWindowHandle = i_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_wheel_x;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(i_lParam), GET_Y_LPARAM(i_lParam));
		inputEvent.mMouse.mWheelValue = (float)GET_WHEEL_DELTA_WPARAM(i_wParam) / WHEEL_DELTA;;
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_MOUSEWHEEL:
		inputEvent.mWindowHandle = i_hwnd;
		inputEvent.mType = RawInputEvent::Type::mouse_wheel_y;
		inputEvent.mMouse.mPosition = Vector2i(GET_X_LPARAM(i_lParam), GET_Y_LPARAM(i_lParam));
		inputEvent.mMouse.mWheelValue = (float)GET_WHEEL_DELTA_WPARAM(i_wParam) / WHEEL_DELTA;;
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_KEYDOWN:
		inputEvent.mWindowHandle = i_hwnd;
		inputEvent.mType = RawInputEvent::Type::key_down;
		inputEvent.mKeyboard.mKey = windowsScanCodeToKey((uint)(i_lParam & 0x00ff0000) >> 16);
		inputEvent.mKeyboard.mRepeat = i_lParam & KF_REPEAT;
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_KEYUP:
		inputEvent.mWindowHandle = i_hwnd;
		inputEvent.mType = RawInputEvent::Type::key_up;
		inputEvent.mKeyboard.mKey = windowsScanCodeToKey((uint)(i_lParam & 0x00ff0000) >> 16);
		inputEvent.mKeyboard.mRepeat = false;
		GE.input().processRawEvent(inputEvent);
		return 0;
	case WM_CHAR:
		inputEvent.mWindowHandle = i_hwnd;
		inputEvent.mType = RawInputEvent::Type::character;
		inputEvent.mKeyboard.mCharacter = (char)i_wParam;
		inputEvent.mKeyboard.mRepeat = i_lParam & KF_REPEAT;
		GE.input().processRawEvent(inputEvent);
		return 0;
	default:
		return DefWindowProc(i_hwnd, i_message, i_wParam, i_lParam);
	}
}