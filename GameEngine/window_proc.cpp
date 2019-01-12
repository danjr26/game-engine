#include "window_proc.h"
#include "game_engine.h"
#include "raw_input_event.h"
#include <windowsx.h>

LRESULT CALLBACK WindowProc(HWND in_hwnd, UINT in_message, WPARAM in_wParam, LPARAM in_lParam) {
	RawInputEvent inputEvent;
	ubyte xButton;
	switch (in_message) {
	case WM_DESTROY:
		GE.Exit();
		return 0;
	case WM_LBUTTONDOWN:
		inputEvent.windowHandle = in_hwnd;
		inputEvent.type = RawInputEvent::Type::mouse_down;
		inputEvent.mouse.button = MouseButton::left;
		inputEvent.mouse.position = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mouse.wheelValue = 0;
		GE.Input().Process_Raw_Event(inputEvent);
		return 0;
	case WM_LBUTTONUP:
		inputEvent.windowHandle = in_hwnd;
		inputEvent.type = RawInputEvent::Type::mouse_up;
		inputEvent.mouse.button = MouseButton::left;
		inputEvent.mouse.position = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mouse.wheelValue = 0;
		GE.Input().Process_Raw_Event(inputEvent);
		return 0;
	case WM_MBUTTONDOWN:
		inputEvent.windowHandle = in_hwnd;
		inputEvent.type = RawInputEvent::Type::mouse_down;
		inputEvent.mouse.button = MouseButton::middle;
		inputEvent.mouse.position = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mouse.wheelValue = 0;
		GE.Input().Process_Raw_Event(inputEvent);
		return 0;
	case WM_MBUTTONUP:
		inputEvent.windowHandle = in_hwnd;
		inputEvent.type = RawInputEvent::Type::mouse_up;
		inputEvent.mouse.button = MouseButton::middle;
		inputEvent.mouse.position = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mouse.wheelValue = 0;
		GE.Input().Process_Raw_Event(inputEvent);
		return 0;
	case WM_RBUTTONDOWN:inputEvent.windowHandle = in_hwnd;
		inputEvent.type = RawInputEvent::Type::mouse_down;
		inputEvent.mouse.button = MouseButton::right;
		inputEvent.mouse.position = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mouse.wheelValue = 0;
		GE.Input().Process_Raw_Event(inputEvent);
		return 0;
	case WM_RBUTTONUP:
		inputEvent.windowHandle = in_hwnd;
		inputEvent.type = RawInputEvent::Type::mouse_up;
		inputEvent.mouse.button = MouseButton::right;
		inputEvent.mouse.position = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mouse.wheelValue = 0;
		GE.Input().Process_Raw_Event(inputEvent);
		return 0;
	case WM_XBUTTONDOWN:
		xButton = (ubyte)GET_XBUTTON_WPARAM(in_wParam);
		if (xButton == 1) {
			inputEvent.windowHandle = in_hwnd;
			inputEvent.type = RawInputEvent::Type::mouse_down;
			inputEvent.mouse.button = MouseButton::extra1;
			inputEvent.mouse.position = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
			inputEvent.mouse.wheelValue = 0;
			GE.Input().Process_Raw_Event(inputEvent);
			return 0;
		}
		else if (xButton == 2) {
			inputEvent.windowHandle = in_hwnd;
			inputEvent.type = RawInputEvent::Type::mouse_down;
			inputEvent.mouse.button = MouseButton::extra2;
			inputEvent.mouse.position = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
			inputEvent.mouse.wheelValue = 0;
			GE.Input().Process_Raw_Event(inputEvent);
			return 0;
		}
		return DefWindowProc(in_hwnd, in_message, in_wParam, in_lParam);
	case WM_XBUTTONUP:
		xButton = (ubyte)GET_XBUTTON_WPARAM(in_wParam);
		if (xButton == 1) {
			inputEvent.windowHandle = in_hwnd;
			inputEvent.type = RawInputEvent::Type::mouse_up;
			inputEvent.mouse.button = MouseButton::extra1;
			inputEvent.mouse.position = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
			inputEvent.mouse.wheelValue = 0;
			GE.Input().Process_Raw_Event(inputEvent);
			return 0;
		}
		else if (xButton == 2) {
			inputEvent.windowHandle = in_hwnd;
			inputEvent.type = RawInputEvent::Type::mouse_up;
			inputEvent.mouse.button = MouseButton::extra2;
			inputEvent.mouse.position = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
			inputEvent.mouse.wheelValue = 0;
			GE.Input().Process_Raw_Event(inputEvent);
			return 0;
		}
		return DefWindowProc(in_hwnd, in_message, in_wParam, in_lParam);
	case WM_MOUSEMOVE:
		inputEvent.windowHandle = in_hwnd;
		inputEvent.type = RawInputEvent::Type::mouse_move;
		inputEvent.mouse.button = MouseButton::none;
		inputEvent.mouse.position = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mouse.wheelValue = 0;
		GE.Input().Process_Raw_Event(inputEvent);
		return 0;
	case WM_MOUSEHWHEEL:
		inputEvent.windowHandle = in_hwnd;
		inputEvent.type = RawInputEvent::Type::mouse_wheel_x;
		inputEvent.mouse.button = MouseButton::none;
		inputEvent.mouse.position = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mouse.wheelValue = (float)GET_WHEEL_DELTA_WPARAM(in_wParam) / WHEEL_DELTA;;
		GE.Input().Process_Raw_Event(inputEvent);
		return 0;
	case WM_MOUSEWHEEL:
		inputEvent.windowHandle = in_hwnd;
		inputEvent.type = RawInputEvent::Type::mouse_wheel_y;
		inputEvent.mouse.button = MouseButton::none;
		inputEvent.mouse.position = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		inputEvent.mouse.wheelValue = (float)GET_WHEEL_DELTA_WPARAM(in_wParam) / WHEEL_DELTA;;
		GE.Input().Process_Raw_Event(inputEvent);
		return 0;
	case WM_KEYDOWN:
		inputEvent.windowHandle = in_hwnd;
		inputEvent.type = RawInputEvent::Type::key_down;
		inputEvent.keyboard.key = Windows_VK_To_Key((uint)in_wParam);
		inputEvent.keyboard.repeat = in_lParam & (1 << 30);
		GE.Input().Process_Raw_Event(inputEvent);
		return 0;
	case WM_KEYUP:
		inputEvent.windowHandle = in_hwnd;
		inputEvent.type = RawInputEvent::Type::key_up;
		inputEvent.keyboard.key = Windows_VK_To_Key((uint)in_wParam);
		inputEvent.keyboard.repeat = false;
		GE.Input().Process_Raw_Event(inputEvent);
		return 0;
	case WM_CHAR:
		inputEvent.windowHandle = in_hwnd;
		inputEvent.type = RawInputEvent::Type::character;
		inputEvent.keyboard.character = (char)in_wParam;
		inputEvent.keyboard.repeat = in_lParam & (1 << 30);
		GE.Input().Process_Raw_Event(inputEvent);
		return 0;
	default:
		return DefWindowProc(in_hwnd, in_message, in_wParam, in_lParam);
	}
}