#ifndef EVENTS_H
#define EVENTS_H
#include <Windows.h>
#include <windowsx.h>
#include "component.h"
#include "trq.h"

LRESULT CALLBACK WindowProc(HWND in_hwnd, UINT in_message, WPARAM in_wParam, LPARAM in_lParam);

class InputContext {
	friend class InputManager;
private:
	uint inputManagerArrayIndex;
public:
	enum Priority {
		game,
		menu,
		reserved
	} priority; //higher value = higher priority

					InputContext			(Priority in_priority);
	virtual			~InputContext			();

	//return true to "eat" input
	virtual bool	Handle_Keyboard_Input	(uint in_message, ulong in_key);
	virtual bool	Handle_Mouse_Input		(uint in_message, Vector2i in_position, float in_wheelDelta);
};

class InputTR;

class InputManager {
public:
	static const uint CAPACITY = 8;
private:
	MSG			message;
	InputTR*	timeRequest;

	IndexedArray<InputContext>
				contexts;

public:
			InputManager			(double in_step);
			~InputManager			();
	void	Process_Input			();
	void	Add_Context				(InputContext* in_context);
	void	Remove_Context			(InputContext* in_context);
	void	Handle_Keyboard_Input	(uint in_message, ulong in_key);
	void	Handle_Mouse_Input		(uint in_message, Vector2i in_position, float in_wheelDelta);
	bool	Is_Down					(uint in_key);
};

Vector2i Get_Mouse_Coords_Pixel		();
Vector2d Pixel_To_Gradient_Coords	(Vector2i in_pixelCoords);
Vector2d Get_Mouse_Coords_Gradient	();
Vector2i Gradient_To_Pixel_Coords	(Vector2d in_gradientCoords);

#endif
