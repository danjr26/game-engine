#include "input.h"
#include "framework.h"

LRESULT CALLBACK WindowProc(HWND in_hwnd, UINT in_message, WPARAM in_wParam, LPARAM in_lParam) {
	Vector2i position;
	float wheelDelta = 0.0f;
	switch (in_message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
		wheelDelta = (float)GET_WHEEL_DELTA_WPARAM(in_wParam) / WHEEL_DELTA;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEMOVE:	
		position = Vector2i(GET_X_LPARAM(in_lParam), GET_Y_LPARAM(in_lParam));
		GEngine::Get().Input().Handle_Mouse_Input(in_message, position, wheelDelta);
		break;

	case WM_KEYDOWN:
		if (in_lParam & (1 << 30)) {
			//30th bit indicates if this is an autorepeat message from holding down the key
			break;
		}
	case WM_KEYUP:
		GEngine::Get().Input().Handle_Keyboard_Input(in_message, (ulong)in_wParam);
		break;

	default:
		return DefWindowProc(in_hwnd, in_message, in_wParam, in_lParam);
	}

	return 0;
}

InputContext::InputContext(Priority in_priority) :
	priority(in_priority)
{}

InputContext::~InputContext() 
{}

bool InputContext::Handle_Keyboard_Input(uint in_message, ulong in_key) {
	return false;
}

bool InputContext::Handle_Mouse_Input(uint in_message, Vector2i in_position, float in_wheelDelta) {
	return false;
}

InputManager::InputManager(double in_step) :
timeRequest(nullptr),
contexts(CAPACITY, offsetof(InputContext, inputManagerArrayIndex), true) {
	timeRequest = new InputTR(&GEngine::Get().Engine_Clock(), in_step);
}

InputManager::~InputManager() {
	delete timeRequest;
}

void InputManager::Process_Input() {
	while (PeekMessageW(&message, GEngine::Get().Window().Get_Handle(), 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
		if (message.message == WM_QUIT) {
#ifdef GENG_PERF
			Profiler::Write();
#endif
			exit(0);
		}
	}
}

void InputManager::Add_Context(InputContext* in_context) {
	for (uint i = 0; i < contexts.Size(); i++) {
		if (in_context->priority >= contexts[i]->priority) {
			contexts.Insert(in_context, i);
			return;
		}
	}
	contexts.Add(in_context);
}

void InputManager::Remove_Context(InputContext* in_context) {
	contexts.Remove(in_context);
}

void InputManager::Handle_Keyboard_Input(uint in_message, ulong in_key) {
	for(uint i=0; i < contexts.Size() && !contexts[i]->Handle_Keyboard_Input(in_message, in_key); i++)
	{ }
}

void InputManager::Handle_Mouse_Input(uint in_message, Vector2i in_position, float in_wheelDelta) {
	for(uint i=0; i < contexts.Size() && !contexts[i]->Handle_Mouse_Input(in_message, in_position, in_wheelDelta); i++)
	{ }
}

bool InputManager::Is_Down(uint in_key) {
	return GetKeyState(in_key) & 0x8000;
}

Vector2i Get_Mouse_Coords_Pixel() {
	POINT offset = { 0, 0 };
	POINT pcoords;
	GetCursorPos(&pcoords);
	ScreenToClient(GEngine::Get().Window().Get_Handle(), &offset);
	return Vector2i(pcoords.x + offset.x, pcoords.y + offset.y);
}

Vector2d Pixel_To_Gradient_Coords(Vector2i in_pixelCoords) {
	Vector2i windowDimensions = GEngine::Get().Window().Get_Dimensions();
	return Vector2d(
		(double)in_pixelCoords.x / windowDimensions.x * 2.0 - 1.0,
		(windowDimensions.y - in_pixelCoords.y) / windowDimensions.y * 2.0 - 1.0
		);
}

Vector2d Get_Mouse_Coords_Gradient() {
	return Pixel_To_Gradient_Coords(Get_Mouse_Coords_Pixel());
}

Vector2i Gradient_To_Pixel_Coords(Vector2d in_gradientCoords) {
	Vector2i windowDimensions = GEngine::Get().Window().Get_Dimensions();
	return Vector2d(
		(in_gradientCoords.x + 1.0) / 2.0 * windowDimensions.x,
		windowDimensions.y - ((in_gradientCoords.y + 1.0) / 2.0 * windowDimensions.y)
		);
}

