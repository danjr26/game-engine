#ifndef EVENTS_H
#define EVENTS_H
#include <windows.h>
#include <windowsx.h>
#include "component.h"
#include "trq.h"

LRESULT CALLBACK WindowProc(HWND i_hwnd, UINT i_message, WPARAM i_wParam, LPARAM i_lParam);

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

					InputContext			(Priority i_priority);
	virtual			~InputContext			();

	//return true to "eat" input
	virtual bool	Handle_Keyboard_Input	(uint i_message, ulong i_key);
	virtual bool	Handle_Mouse_Input		(uint i_message, Vector2i i_position, float i_wheelDelta);
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
			InputManager			(double i_step);
			~InputManager			();
	void	Process_Input			();
	void	Add_Context				(InputContext* i_context);
	void	Remove_Context			(InputContext* i_context);
	void	Handle_Keyboard_Input	(uint i_message, ulong i_key);
	void	Handle_Mouse_Input		(uint i_message, Vector2i i_position, float i_wheelDelta);
	bool	isDown					(uint i_key);
};

Vector2i Get_Mouse_Coords_Pixel		();
Vector2d Pixel_To_Gradient_Coords	(Vector2i i_pixelCoords);
Vector2d Get_Mouse_Coords_Gradient	();
Vector2i Gradient_To_Pixel_Coords	(Vector2d i_gradientCoords);

#endif
