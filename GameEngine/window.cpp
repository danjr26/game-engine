#include "window.h"
#include "log.h"
#include "bit_manipulation.h"
#include "windows_error.h"
#include "resource.h"
#include "window_proc.h"
#include "game_engine.h"

uint Window::classIndex = 1;

Window::Window(Params& in_params) :
mParams(in_params) {
	std::lock_guard<std::mutex> lock(mMutex);
	std::wstring className = std::wstring(L"class") + std::to_wstring(classIndex++);

	HINSTANCE instanceHandle = GetModuleHandle(nullptr);

	mWindowClass.hInstance = instanceHandle;
	mWindowClass.lpszClassName = className.c_str();
	mWindowClass.lpfnWndProc = WindowProc;
	mWindowClass.style = CS_DBLCLKS | CS_OWNDC;
	mWindowClass.cbSize = sizeof(WNDCLASSEX);
	mWindowClass.hIcon = LoadIcon(instanceHandle, MAKEINTRESOURCE(IDI_ICON1));
	mWindowClass.hIconSm = LoadIcon(instanceHandle, MAKEINTRESOURCE(IDI_ICON1));
	mWindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	mWindowClass.lpszMenuName =	nullptr;
	mWindowClass.cbClsExtra = 0;
	mWindowClass.cbWndExtra = 0;
	mWindowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	if (!RegisterClassEx(&mWindowClass)) {
		throw ProcessFailureException(std::string("failed to create window class with error message:\n") + getWindowsErrorMessage());
	}

	DWORD style = (mParams.mFullscreen) ?
		WS_POPUP | (mParams.mStyle & WS_VISIBLE) | (mParams.mStyle & WS_MINIMIZE) :
		mParams.mStyle;

	RECT rect = { 0, 0, mParams.mDimensions.x(), mParams.mDimensions.y() };
	
	if (/*!params.fullscreen && */!AdjustWindowRectEx((LPRECT)&rect, style, false, mParams.mExStyle)) {
		throw ProcessFailureException(std::string("failed to create window with error message:\n") + getWindowsErrorMessage());
	}

	mWindowHandle = CreateWindowEx(
		mParams.mExStyle,                   
		mWindowClass.lpszClassName,
		mParams.mName.c_str(),
		style,
		mParams.mPosition.x(),       
		mParams.mPosition.y(),       
		rect.right - rect.left,                
		rect.bottom - rect.top,                 
		HWND_DESKTOP,        
		nullptr,	// no menu            
		instanceHandle,      
		nullptr		// no extra data
	);

	setFullscreen(mParams.mFullscreen);

	mDeviceContextHandle = GetDC(mWindowHandle);

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,	// version of data structure (has to be 1)
		PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL,
		PFD_TYPE_RGBA,
		mParams.mColorBits, //color buffer
		0, 0, 0, 0, 0, 0, 0, 0, // bitplanes, shift (not used)
		0,
		0, 0, 0, 0,
		mParams.mDepthBits, // depth buffer
		mParams.mStencilBits,	// stencil buffer
		0,
		0,
		PFD_MAIN_PLANE,	//layer type
		0, 0, 0
	};

	int pixelFormat = ChoosePixelFormat(mDeviceContextHandle, &pfd);

	SetPixelFormat(mDeviceContextHandle, pixelFormat, &pfd);
	DescribePixelFormat(mDeviceContextHandle, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	mParams.mColorBits = pfd.cColorBits;
	mParams.mDepthBits = pfd.cDepthBits;
	mParams.mStencilBits = pfd.cStencilBits;

	mRenderContext = wglCreateContext(mDeviceContextHandle);
	wglMakeCurrent(mDeviceContextHandle, mRenderContext);

	glewInit();

	GE.windows().add(this);
}

Window::~Window() {
	std::lock_guard<std::mutex> lock(mMutex);
	if (GameEngine::exists()) {
		GE.windows().remove(this);
	}
	setFullscreen(false);
	DestroyWindow(mWindowHandle);
	wglDeleteContext(mRenderContext);
}

void Window::flipBuffers() {
	std::lock_guard<std::mutex> lock(mMutex);
	SwapBuffers(mDeviceContextHandle);
}

std::wstring Window::getName() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mName;
}

Vector2i Window::getPosition() {
	std::lock_guard<std::mutex> lock(mMutex);
	RECT rect;
	GetClientRect(mWindowHandle, &rect);
	return Vector2i(rect.left, rect.top);
}

Vector2ui Window::getDimensions() const {
	std::lock_guard<std::mutex> lock(mMutex);
	RECT rect;
	GetClientRect(mWindowHandle, &rect);
	return Vector2ui(rect.right - rect.left, rect.bottom - rect.top);
}

bool Window::isFullscreen() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mFullscreen;
}

bool Window::isAlwaysFront() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mExStyle & WS_EX_TOPMOST;
}

bool Window::hasTitleBar() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_CAPTION;
}

bool Window::hasBorder() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_BORDER;
}

bool Window::isActivatable() {
	std::lock_guard<std::mutex> lock(mMutex);
	return !(mParams.mExStyle & WS_EX_NOACTIVATE);
}

bool Window::isVisible() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_VISIBLE;
}

bool Window::isMinimized() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_MINIMIZE;
}

bool Window::isMaximized() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_MAXIMIZE;
}

bool Window::hasCloseButton() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_SYSMENU;
}

bool Window::hasMinimizeButton() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_MINIMIZEBOX;
}

bool Window::hasMaximizeButton() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_MAXIMIZEBOX;
}

bool Window::isResizable() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_SIZEBOX;
}

bool Window::hasVerticalScrollBar() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_VSCROLL;
}

bool Window::hasHorizontalScrollBar() {
	return mParams.mStyle & WS_HSCROLL;
}

BYTE Window::getColorBits() {
	return mParams.mColorBits;
}

BYTE Window::getDepthBits() {
	return mParams.mDepthBits;
}

BYTE Window::getStencilBits() {
	return mParams.mStencilBits;
}

void Window::setPosition(Vector2i in_position) {
	std::lock_guard<std::mutex> lock(mMutex);
	SetWindowPos(mWindowHandle, 0, in_position.x(), in_position.y(), 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Window::setDimensions(Vector2i in_dimensions) {
	std::lock_guard<std::mutex> lock(mMutex);
	SetWindowPos(mWindowHandle, 0, 0, 0, in_dimensions.x(), in_dimensions.y(), SWP_NOMOVE | SWP_NOZORDER);
}

void Window::setFullscreen(bool in_value) {
	mParams.mFullscreen = in_value;
	if (mParams.mFullscreen) {
		DEVMODE deviceMode;
		deviceMode.dmSize = sizeof(DEVMODE);
		deviceMode.dmPelsWidth = mParams.mDimensions.x();
		deviceMode.dmPelsHeight = mParams.mDimensions.y();
		deviceMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFIXEDOUTPUT;
		deviceMode.dmDisplayFixedOutput = DMDFO_DEFAULT;

		ChangeDisplaySettingsEx(NULL, &deviceMode, NULL, CDS_FULLSCREEN, NULL);
	}
	else {
		ChangeDisplaySettingsEx(NULL, NULL, NULL, 0, NULL);
	}
}

void Window::setTopmost(bool in_value) {}

void Window::setVisible(bool in_value) {
	std::lock_guard<std::mutex> lock(mMutex);
	ShowWindow(mWindowHandle, (in_value) ? SW_SHOW : SW_HIDE);
}

HWND Window::getHandle() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mWindowHandle;
}

void Window::setName(const std::wstring& in_name) {
	std::lock_guard<std::mutex> lock(mMutex);
	SetWindowText(mWindowHandle, in_name.c_str());
}

Window::Params::Params() :
	mDimensions(800, 600),
	mPosition(CW_USEDEFAULT, CW_USEDEFAULT),
	mName(L"Window"),
	mStyle(WS_CAPTION | WS_BORDER | WS_VISIBLE | WS_SYSMENU),
	mExStyle(WS_EX_APPWINDOW),
	mFullscreen(false),
	mColorBits(32),
	mDepthBits(32),
	mStencilBits(0)
{}

Window::Params& Window::Params::Name(const std::wstring& in_name) {
	mName = in_name;
	return (*this);
}

Window::Params& Window::Params::Position(const Vector2i& in_position) {
	mPosition = in_position;
	return (*this);
}

Window::Params& Window::Params::Dimensions(const Vector2i& in_dimensions) {
	mDimensions = in_dimensions;
	return (*this);
}

Window::Params& Window::Params::Fullscreen(bool in_value) {
	mFullscreen = in_value;
	return (*this);
}

Window::Params& Window::Params::Always_Front(bool in_value) {
	setBit<DWORD>(mExStyle, WS_EX_TOPMOST, in_value);
	return (*this);
}

Window::Params& Window::Params::Title_Bar(bool in_value) {
	setBit<DWORD>(mStyle, WS_CAPTION, in_value);
	return (*this);
}

Window::Params& Window::Params::Border(bool in_value) {
	setBit<DWORD>(mStyle, WS_BORDER, in_value);
	return (*this);
}

Window::Params& Window::Params::Activatable(bool in_value) {
	setBit<DWORD>(mExStyle, WS_EX_NOACTIVATE, !in_value);
	return (*this);
}

Window::Params& Window::Params::Visible(bool in_value) {
	setBit<DWORD>(mStyle, WS_VISIBLE, in_value);
	return (*this);
}

Window::Params& Window::Params::Minimized(bool in_value) {
	setBit<DWORD>(mStyle, WS_MINIMIZE, in_value);
	return (*this);
}

Window::Params& Window::Params::Maximized(bool in_value) {
	setBit<DWORD>(mStyle, WS_MAXIMIZE, in_value);
	return (*this);
}

Window::Params& Window::Params::Close_Button(bool in_value) {
	setBit<DWORD>(mStyle, WS_SYSMENU, in_value);
	return (*this);
}

Window::Params& Window::Params::Minimize_Button(bool in_value) {
	setBit<DWORD>(mStyle, WS_MINIMIZEBOX, in_value);
	return (*this);
}

Window::Params& Window::Params::Maximize_Button(bool in_value) {
	setBit<DWORD>(mStyle, WS_MAXIMIZEBOX, in_value);
	return (*this);
}

Window::Params& Window::Params::Resizable(bool in_value) {
	setBit<DWORD>(mStyle, WS_SIZEBOX, in_value);
	return (*this);
}

Window::Params& Window::Params::Color_Bits(BYTE in_value) {
	mColorBits = in_value;
	return (*this);
}

Window::Params& Window::Params::Depth_Bits(BYTE in_value) {
	mDepthBits = in_value;
	return (*this);
}

Window::Params& Window::Params::Stencil_Bits(BYTE in_value) {
	mStencilBits = in_value;
	return (*this);
}

std::vector<Window::DisplayMode> Window::getSupportedDisplayModes() {
	std::vector<DisplayMode> displayModes;
	DEVMODE deviceMode;
	DisplayMode displayMode;
	for (uint i = 0; EnumDisplaySettings(NULL, i, &deviceMode) != 0; i++) {
		displayMode.mDimensions = Vector2i(deviceMode.dmPelsWidth, deviceMode.dmPelsHeight);
		displayMode.mColorBits = deviceMode.dmBitsPerPel;
		displayMode.mFrequency = deviceMode.dmDisplayFrequency;
		displayModes.push_back(displayMode);
	}
	return displayModes;
}

void Window::_drawToThis() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, mParams.mDimensions.x(), mParams.mDimensions.y());
}
