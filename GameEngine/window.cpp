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

	mWindowClass.hInstance = mParams.mInstanceHandle;
	mWindowClass.lpszClassName = className.c_str();
	mWindowClass.lpfnWndProc = WindowProc;
	mWindowClass.style = CS_DBLCLKS | CS_OWNDC;
	mWindowClass.cbSize = sizeof(WNDCLASSEX);
	mWindowClass.hIcon = LoadIcon(mParams.mInstanceHandle, MAKEINTRESOURCE(IDI_ICON1));
	mWindowClass.hIconSm = LoadIcon(mParams.mInstanceHandle, MAKEINTRESOURCE(IDI_ICON1));
	mWindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	mWindowClass.lpszMenuName =	nullptr;
	mWindowClass.cbClsExtra = 0;
	mWindowClass.cbWndExtra = 0;
	mWindowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	if (!RegisterClassEx(&mWindowClass)) {
		throw ProcessFailureException(std::string("failed to create window class with error message:\n") + Get_Windows_Error_Message());
	}

	DWORD style = (mParams.mFullscreen) ?
		WS_POPUP | (mParams.mStyle & WS_VISIBLE) | (mParams.mStyle & WS_MINIMIZE) :
		mParams.mStyle;

	RECT rect = { 0, 0, mParams.mDimensions.X(), mParams.mDimensions.Y() };
	
	if (/*!params.fullscreen && */!AdjustWindowRectEx((LPRECT)&rect, style, false, mParams.mExStyle)) {
		throw ProcessFailureException(std::string("failed to create window with error message:\n") + Get_Windows_Error_Message());
	}

	mWindowHandle = CreateWindowEx(
		mParams.mExStyle,                   
		mWindowClass.lpszClassName,
		mParams.mName.c_str(),
		style,
		mParams.mPosition.X(),       
		mParams.mPosition.Y(),       
		rect.right - rect.left,                
		rect.bottom - rect.top,                 
		HWND_DESKTOP,        
		nullptr,	// no menu            
		mParams.mInstanceHandle,      
		nullptr		// no extra data
	);

	Set_Fullscreen(mParams.mFullscreen);

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

	GE.Windows().Add(this);
}

Window::~Window() {
	std::lock_guard<std::mutex> lock(mMutex);
	GE.Windows().Remove(this);
	DestroyWindow(mWindowHandle);
	Set_Fullscreen(false);
}

void Window::Flip_Buffers() {
	std::lock_guard<std::mutex> lock(mMutex);
	SwapBuffers(mDeviceContextHandle);
}

std::wstring Window::Get_Name() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mName;
}

Vector2i Window::Get_Position() {
	std::lock_guard<std::mutex> lock(mMutex);
	RECT rect;
	GetClientRect(mWindowHandle, &rect);
	return Vector2i(rect.left, rect.top);
}

Vector2i Window::Get_Dimensions() const {
	std::lock_guard<std::mutex> lock(mMutex);
	RECT rect;
	GetClientRect(mWindowHandle, &rect);
	return Vector2i(rect.right - rect.left, rect.bottom - rect.top);
}

bool Window::Is_Fullscreen() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mFullscreen;
}

bool Window::Is_Always_Front() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mExStyle & WS_EX_TOPMOST;
}

bool Window::Has_Title_Bar() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_CAPTION;
}

bool Window::Has_Border() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_BORDER;
}

bool Window::Is_Activatable() {
	std::lock_guard<std::mutex> lock(mMutex);
	return !(mParams.mExStyle & WS_EX_NOACTIVATE);
}

bool Window::Is_Visible() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_VISIBLE;
}

bool Window::Is_Minimized() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_MINIMIZE;
}

bool Window::Is_Maximized() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_MAXIMIZE;
}

bool Window::Has_Close_Button() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_SYSMENU;
}

bool Window::Has_Minimize_Button() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_MINIMIZEBOX;
}

bool Window::Has_Maximize_Button() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_MAXIMIZEBOX;
}

bool Window::Is_Resizable() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_SIZEBOX;
}

bool Window::Has_Vertical_Scroll_Bar() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mParams.mStyle & WS_VSCROLL;
}

bool Window::Has_Horizontal_Scroll_Bar() {
	return mParams.mStyle & WS_HSCROLL;
}

BYTE Window::Get_Color_Bits() {
	return mParams.mColorBits;
}

BYTE Window::Get_Depth_Bits() {
	return mParams.mDepthBits;
}

BYTE Window::Get_Stencil_Bits() {
	return mParams.mStencilBits;
}

void Window::Set_Position(Vector2i in_position) {
	std::lock_guard<std::mutex> lock(mMutex);
	SetWindowPos(mWindowHandle, 0, in_position.X(), in_position.Y(), 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Window::Set_Dimensions(Vector2i in_dimensions) {
	std::lock_guard<std::mutex> lock(mMutex);
	SetWindowPos(mWindowHandle, 0, 0, 0, in_dimensions.X(), in_dimensions.Y(), SWP_NOMOVE | SWP_NOZORDER);
}

void Window::Set_Fullscreen(bool in_value) {
	mParams.mFullscreen = in_value;
	if (mParams.mFullscreen) {
		DEVMODE deviceMode;
		deviceMode.dmSize = sizeof(DEVMODE);
		deviceMode.dmPelsWidth = mParams.mDimensions.X();
		deviceMode.dmPelsHeight = mParams.mDimensions.Y();
		deviceMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFIXEDOUTPUT;
		deviceMode.dmDisplayFixedOutput = DMDFO_DEFAULT;

		ChangeDisplaySettingsEx(NULL, &deviceMode, NULL, CDS_FULLSCREEN, NULL);
	}
	else {
		ChangeDisplaySettingsEx(NULL, NULL, NULL, 0, NULL);
	}
}

void Window::Set_Topmost(bool in_value) {}

void Window::Set_Visible(bool in_value) {
	std::lock_guard<std::mutex> lock(mMutex);
	ShowWindow(mWindowHandle, (in_value) ? SW_SHOW : SW_HIDE);
}

HWND Window::Get_Handle() {
	std::lock_guard<std::mutex> lock(mMutex);
	return mWindowHandle;
}

void Window::Set_Name(const std::wstring& in_name) {
	std::lock_guard<std::mutex> lock(mMutex);
	SetWindowText(mWindowHandle, in_name.c_str());
}

Window::Params::Params(HINSTANCE in_instance) :
	mInstanceHandle(in_instance),
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
	Set_Bit<DWORD>(mExStyle, WS_EX_TOPMOST, in_value);
	return (*this);
}

Window::Params& Window::Params::Title_Bar(bool in_value) {
	Set_Bit<DWORD>(mStyle, WS_CAPTION, in_value);
	return (*this);
}

Window::Params& Window::Params::Border(bool in_value) {
	Set_Bit<DWORD>(mStyle, WS_BORDER, in_value);
	return (*this);
}

Window::Params& Window::Params::Activatable(bool in_value) {
	Set_Bit<DWORD>(mExStyle, WS_EX_NOACTIVATE, !in_value);
	return (*this);
}

Window::Params& Window::Params::Visible(bool in_value) {
	Set_Bit<DWORD>(mStyle, WS_VISIBLE, in_value);
	return (*this);
}

Window::Params& Window::Params::Minimized(bool in_value) {
	Set_Bit<DWORD>(mStyle, WS_MINIMIZE, in_value);
	return (*this);
}

Window::Params& Window::Params::Maximized(bool in_value) {
	Set_Bit<DWORD>(mStyle, WS_MAXIMIZE, in_value);
	return (*this);
}

Window::Params& Window::Params::Close_Button(bool in_value) {
	Set_Bit<DWORD>(mStyle, WS_SYSMENU, in_value);
	return (*this);
}

Window::Params& Window::Params::Minimize_Button(bool in_value) {
	Set_Bit<DWORD>(mStyle, WS_MINIMIZEBOX, in_value);
	return (*this);
}

Window::Params& Window::Params::Maximize_Button(bool in_value) {
	Set_Bit<DWORD>(mStyle, WS_MAXIMIZEBOX, in_value);
	return (*this);
}

Window::Params& Window::Params::Resizable(bool in_value) {
	Set_Bit<DWORD>(mStyle, WS_SIZEBOX, in_value);
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

std::vector<Window::DisplayMode> Window::Get_Supported_Display_Modes() {
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

void Window::_Draw_To_This() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, mParams.mDimensions.X(), mParams.mDimensions.Y());
}
