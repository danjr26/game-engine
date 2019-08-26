#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include <mutex>
#include <vector>
#include "render_target.h"

class Window : public RenderTarget {
public: 
	class Params {
		friend class Window;
	private:
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
		Params();
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

	void flipBuffers();
	std::wstring getName();
	Vector2i getPosition();
	Vector2ui getDimensions() const;
	bool isFullscreen();
	bool isAlwaysFront();
	bool hasTitleBar();
	bool hasBorder();
	bool isActivatable();
	bool isVisible();
	bool isMinimized();
	bool isMaximized();
	bool hasCloseButton();
	bool hasMinimizeButton();
	bool hasMaximizeButton();
	bool isResizable();
	bool hasVerticalScrollBar();
	bool hasHorizontalScrollBar();
	BYTE getColorBits();
	BYTE getDepthBits();
	BYTE getStencilBits();
	HWND getHandle();
	void setName(const std::wstring& in_name);
	void setPosition(Vector2i in_position);
	void setDimensions(Vector2i in_dimensions);
	void setFullscreen(bool in_value = true);
	void setTopmost(bool in_value = true);
	void setVisible(bool in_value = true);
	void setActivatable(bool in_value = true);
	void setMinimized(bool in_value = true);
	void setMaximized(bool in_value = true);
	void bringToFront();
	void sendToBack();

	static std::vector<DisplayMode> getSupportedDisplayModes();

protected:
	void _drawToThis() override final;
};

#endif