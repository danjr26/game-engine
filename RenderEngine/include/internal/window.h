#ifndef WINDOW_H
#define WINDOW_H

#include "render_target.h"
#include "enum_wrappers.h"

class Window : public RenderTarget {
public:
	using InputCallback = LRESULT (*)(HWND, UINT, WPARAM, LPARAM);

	struct PixelFormat {
		uint mNColorBits;
		uint mNDepthBits;
		uint mNStencilBits;
	};

private:
	static const DWORD defaultStyle;
	static const DWORD defaultStyleEx;
	static const Vector2ui defaultDimen;
	static const std::string defaultName;
	static const PixelFormat defaultFormat;

	HWND mWindow;
	HGLRC mRenderContext;

public:
	Window();
	~Window();

	void init(const PixelFormat& i_format = defaultFormat);

	void flipBuffers();

	void show();
	void unshow();
	bool isShown();

	void maximize();
	void unmaximize();
	bool isMaximized();

	void minimize();
	void unminimize();
	bool isMinimized();

	Vector2i getPosition() const;
	void setPosition(const Vector2i& i_position);

	Vector2ui getDimen() const;
	void setDimen(const Vector2ui& i_dimen);

	std::string getName() const;
	void setName(const std::string& i_name);

	void setInputCallback(InputCallback i_callback);

private:
	void setStyleBit(LONG i_bit);
	void clearStyleBit(LONG i_bit);
	bool getStyleBit(LONG i_bit);

	void setStyleExBit(LONG i_bit);
	void clearStyleExBit(LONG i_bit);
	bool getStyleExBit(LONG i_bit);

	void _activate() override;
	Vector2ui _getRenderTargetDimen() const override;
};

#endif