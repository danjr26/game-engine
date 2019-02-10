#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <mutex>
#include <vector>
#include "render_target.h"

class Window : public RenderTarget {
public: 
	class Params {
		friend class Window;
	private:
		HINSTANCE mInstanceHandle;
		Vector2i mPosition;
		Vector2i mDimensions;
		std::wstring mName;
		DWORD mStyle;
		DWORD mExStyle;
		BYTE mColorBits;
		BYTE mDepthBits;
		BYTE mStencilBits;
		bool mFullscreen;

	public:
		Params(HINSTANCE in_instance);
		Params& Name(const std::wstring& in_name);
		Params& Position(const Vector2i& in_position);
		Params& Dimensions(const Vector2i& in_dimensions);
		Params& Fullscreen(bool in_value = true);
		Params& Always_Front(bool in_value = true);
		Params& Title_Bar(bool in_value = true);
		Params& Border(bool in_value = true);
		Params& Activatable(bool in_value = true);
		Params& Visible(bool in_value = true);
		Params& Minimized(bool in_value = true);
		Params& Maximized(bool in_value = true);
		Params& Close_Button(bool in_value = true);
		Params& Minimize_Button(bool in_value = true);
		Params& Maximize_Button(bool in_value = true);
		Params& Resizable(bool in_value = true);
		Params& Color_Bits(BYTE in_value);
		Params& Depth_Bits(BYTE in_value);
		Params& Stencil_Bits(BYTE in_value);
	};

	struct DisplayMode {
		Vector2i mDimensions;
		uint mFrequency;
		DWORD mColorBits;
	};

private:
	static uint classIndex;

	mutable std::mutex mMutex;

	HWND mWindowHandle;
	WNDCLASSEX mWindowClass;
	HDC	mDeviceContextHandle;
	HGLRC mRenderContext;
	
	Params mParams;

public:
	Window(Params& in_params);
	~Window();
	void Flip_Buffers();
	std::wstring Get_Name();
	Vector2i Get_Position();
	Vector2i Get_Dimensions() const;
	bool Is_Fullscreen();
	bool Is_Always_Front();
	bool Has_Title_Bar();
	bool Has_Border();
	bool Is_Activatable();
	bool Is_Visible();
	bool Is_Minimized();
	bool Is_Maximized();
	bool Has_Close_Button();
	bool Has_Minimize_Button();
	bool Has_Maximize_Button();
	bool Is_Resizable();
	bool Has_Vertical_Scroll_Bar();
	bool Has_Horizontal_Scroll_Bar();
	BYTE Get_Color_Bits();
	BYTE Get_Depth_Bits();
	BYTE Get_Stencil_Bits();
	HWND Get_Handle();
	void Set_Name(const std::wstring& in_name);
	void Set_Position(Vector2i in_position);
	void Set_Dimensions(Vector2i in_dimensions);
	void Set_Fullscreen(bool in_value = true);
	void Set_Topmost(bool in_value = true);
	void Set_Visible(bool in_value = true);
	void Set_Activatable(bool in_value = true);
	void Set_Minimized(bool in_value = true);
	void Set_Maximized(bool in_value = true);
	void Bring_To_Front();
	void Send_To_Back();

	static std::vector<DisplayMode> Get_Supported_Display_Modes();

protected:
	void _Draw_To_This() override final;
};

#endif