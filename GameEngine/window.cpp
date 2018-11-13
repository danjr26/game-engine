#include "window.h"
#include "log.h"
#include "bit_manipulation.h"
#include "windows_error.h"
#include "resource.h"
#include "window_proc.h"
#include "game_engine.h"

uint Window::classIndex = 1;

Window::Window(Params& in_params) :
params(in_params) {
	std::lock_guard<std::mutex> lock(mutex);
	std::wstring className = std::wstring(L"class") + std::to_wstring(classIndex++);

	windowClass.hInstance = params.hInstance;
	windowClass.lpszClassName = className.c_str();
	windowClass.lpfnWndProc = WindowProc;
	windowClass.style = CS_DBLCLKS | CS_OWNDC;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hIcon = LoadIcon(params.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	windowClass.hIconSm = LoadIcon(params.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszMenuName =	nullptr;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

	if (!RegisterClassEx(&windowClass)) {
		throw ProcessFailureException(std::string("failed to create window class with error message:\n") + Get_Windows_Error_Message());
	}

	RECT rect = { 0, 0, params.dimensions.X(), params.dimensions.Y() };
	
	if (!AdjustWindowRectEx((LPRECT)&rect, params.style, false, params.exStyle)) {
		throw ProcessFailureException(std::string("failed to create window with error message:\n") + Get_Windows_Error_Message());
	}

	hWindow = CreateWindowEx(
		params.exStyle,                   
		windowClass.lpszClassName,
		params.name.c_str(),
		(params.fullscreen) ? 
			WS_POPUP | (params.style & WS_VISIBLE) | (params.style & WS_MINIMIZE) : 
			params.style,
		params.position.X(),       
		params.position.Y(),       
		rect.right - rect.left,                
		rect.bottom - rect.top,                 
		HWND_DESKTOP,        
		nullptr,	// no menu            
		params.hInstance,      
		nullptr		// no extra data
	);

	Set_Fullscreen(params.fullscreen);

	hDeviceContext = GetDC(hWindow);

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,	// version of data structure (has to be 1)
		PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL,
		PFD_TYPE_RGBA,
		params.colorBits, //color buffer
		0, 0, 0, 0, 0, 0, 0, 0, // bitplanes, shift (not used)
		0,
		0, 0, 0, 0,
		params.depthBits, // depth buffer
		params.stencilBits,	// stencil buffer
		0,
		0,
		PFD_MAIN_PLANE,	//layer type
		0, 0, 0
	};

	int pixelFormat = ChoosePixelFormat(hDeviceContext, &pfd);

	SetPixelFormat(hDeviceContext, pixelFormat, &pfd);
	DescribePixelFormat(hDeviceContext, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	params.colorBits = pfd.cColorBits;
	params.depthBits = pfd.cDepthBits;
	params.stencilBits = pfd.cStencilBits;

	hGLRenderContext = wglCreateContext(hDeviceContext);
	wglMakeCurrent(hDeviceContext, hGLRenderContext);

	glewInit();

	GE.Windows().Add(this);
}

Window::~Window() {
	std::lock_guard<std::mutex> lock(mutex);
	GE.Windows().Remove(this);
	DestroyWindow(hWindow);
}

void Window::Flip_Buffers() {
	std::lock_guard<std::mutex> lock(mutex);
	SwapBuffers(hDeviceContext);
}

std::wstring Window::Get_Name() {
	std::lock_guard<std::mutex> lock(mutex);
	return params.name;
}

Vector2i Window::Get_Position() {
	std::lock_guard<std::mutex> lock(mutex);
	RECT rect;
	GetClientRect(hWindow, &rect);
	return Vector2i(rect.left, rect.top);
}

Vector2i Window::Get_Dimensions() const {
	std::lock_guard<std::mutex> lock(mutex);
	RECT rect;
	GetClientRect(hWindow, &rect);
	return Vector2i(rect.right - rect.left, rect.bottom - rect.top);
}

bool Window::Is_Fullscreen() {
	std::lock_guard<std::mutex> lock(mutex);
	return params.fullscreen;
}

bool Window::Is_Always_Front() {
	std::lock_guard<std::mutex> lock(mutex);
	return params.exStyle & WS_EX_TOPMOST;
}

bool Window::Has_Title_Bar() {
	std::lock_guard<std::mutex> lock(mutex);
	return params.style & WS_CAPTION;
}

bool Window::Has_Border() {
	std::lock_guard<std::mutex> lock(mutex);
	return params.style & WS_BORDER;
}

bool Window::Is_Activatable() {
	std::lock_guard<std::mutex> lock(mutex);
	return !(params.exStyle & WS_EX_NOACTIVATE);
}

bool Window::Is_Visible() {
	std::lock_guard<std::mutex> lock(mutex);
	return params.style & WS_VISIBLE;
}

bool Window::Is_Minimized() {
	std::lock_guard<std::mutex> lock(mutex);
	return params.style & WS_MINIMIZE;
}

bool Window::Is_Maximized() {
	std::lock_guard<std::mutex> lock(mutex);
	return params.style & WS_MAXIMIZE;
}

bool Window::Has_Close_Button() {
	std::lock_guard<std::mutex> lock(mutex);
	return params.style & WS_SYSMENU;
}

bool Window::Has_Minimize_Button() {
	std::lock_guard<std::mutex> lock(mutex);
	return params.style & WS_MINIMIZEBOX;
}

bool Window::Has_Maximize_Button() {
	std::lock_guard<std::mutex> lock(mutex);
	return params.style & WS_MAXIMIZEBOX;
}

bool Window::Is_Resizable() {
	std::lock_guard<std::mutex> lock(mutex);
	return params.style & WS_SIZEBOX;
}

bool Window::Has_Vertical_Scroll_Bar() {
	std::lock_guard<std::mutex> lock(mutex);
	return params.style & WS_VSCROLL;
}

bool Window::Has_Horizontal_Scroll_Bar() {
	return params.style & WS_HSCROLL;
}

BYTE Window::Get_Color_Bits() {
	return params.colorBits;
}

BYTE Window::Get_Depth_Bits() {
	return params.depthBits;
}

BYTE Window::Get_Stencil_Bits() {
	return params.stencilBits;
}

void Window::Set_Position(Vector2i in_position) {
	std::lock_guard<std::mutex> lock(mutex);
	SetWindowPos(hWindow, 0, in_position.X(), in_position.Y(), 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Window::Set_Dimensions(Vector2i in_dimensions) {
	std::lock_guard<std::mutex> lock(mutex);
	SetWindowPos(hWindow, 0, 0, 0, in_dimensions.X(), in_dimensions.Y(), SWP_NOMOVE | SWP_NOZORDER);
}

void Window::Set_Fullscreen(bool in_value) {
	params.fullscreen = in_value;
	if (params.fullscreen) {
		DEVMODE deviceMode;
		deviceMode.dmSize = sizeof(DEVMODE);
		deviceMode.dmPelsWidth = params.dimensions.X();
		deviceMode.dmPelsHeight = params.dimensions.Y();
		deviceMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFIXEDOUTPUT;
		deviceMode.dmDisplayFixedOutput = DMDFO_STRETCH;

		ChangeDisplaySettingsEx(NULL, &deviceMode, NULL, CDS_FULLSCREEN | CDS_RESET, NULL);
	}
	else {
		ChangeDisplaySettingsEx(NULL, NULL, NULL, 0, NULL);
	}
}

void Window::Set_Topmost(bool in_value) {}

HWND Window::Get_Handle() {
	std::lock_guard<std::mutex> lock(mutex);
	return hWindow;
}

void Window::Set_Name(const std::wstring& in_name) {
	SetWindowText(hWindow, in_name.c_str());
}

Window::Params::Params(HINSTANCE in_instance) :
	hInstance(in_instance),
	dimensions(800, 600),
	position(CW_USEDEFAULT, CW_USEDEFAULT),
	name(L"Window"),
	style(WS_CAPTION | WS_BORDER | WS_VISIBLE | WS_SYSMENU),
	exStyle(WS_EX_APPWINDOW),
	fullscreen(false),
	colorBits(32),
	depthBits(32),
	stencilBits(0)
{}

Window::Params& Window::Params::Name(const std::wstring& in_name) {
	name = in_name;
	return (*this);
}

Window::Params& Window::Params::Position(const Vector2i& in_position) {
	position = in_position;
	return (*this);
}

Window::Params& Window::Params::Dimensions(const Vector2i& in_dimensions) {
	dimensions = in_dimensions;
	return (*this);
}

Window::Params& Window::Params::Fullscreen(bool in_value) {
	fullscreen = in_value;
	return (*this);
}

Window::Params& Window::Params::Always_Front(bool in_value) {
	Set_Bit<DWORD>(exStyle, WS_EX_TOPMOST, in_value);
	return (*this);
}

Window::Params& Window::Params::Title_Bar(bool in_value) {
	Set_Bit<DWORD>(style, WS_CAPTION, in_value);
	return (*this);
}

Window::Params& Window::Params::Border(bool in_value) {
	Set_Bit<DWORD>(style, WS_BORDER, in_value);
	return (*this);
}

Window::Params& Window::Params::Activatable(bool in_value) {
	Set_Bit<DWORD>(exStyle, WS_EX_NOACTIVATE, !in_value);
	return (*this);
}

Window::Params& Window::Params::Visible(bool in_value) {
	Set_Bit<DWORD>(style, WS_VISIBLE, in_value);
	return (*this);
}

Window::Params& Window::Params::Minimized(bool in_value) {
	Set_Bit<DWORD>(style, WS_MINIMIZE, in_value);
	return (*this);
}

Window::Params& Window::Params::Maximized(bool in_value) {
	Set_Bit<DWORD>(style, WS_MAXIMIZE, in_value);
	return (*this);
}

Window::Params& Window::Params::Close_Button(bool in_value) {
	Set_Bit<DWORD>(style, WS_SYSMENU, in_value);
	return (*this);
}

Window::Params& Window::Params::Minimize_Button(bool in_value) {
	Set_Bit<DWORD>(style, WS_MINIMIZEBOX, in_value);
	return (*this);
}

Window::Params& Window::Params::Maximize_Button(bool in_value) {
	Set_Bit<DWORD>(style, WS_MAXIMIZEBOX, in_value);
	return (*this);
}

Window::Params& Window::Params::Resizable(bool in_value) {
	Set_Bit<DWORD>(style, WS_SIZEBOX, in_value);
	return (*this);
}

Window::Params& Window::Params::Color_Bits(BYTE in_value) {
	colorBits = in_value;
	return (*this);
}

Window::Params& Window::Params::Depth_Bits(BYTE in_value) {
	depthBits = in_value;
	return (*this);
}

Window::Params& Window::Params::Stencil_Bits(BYTE in_value) {
	stencilBits = in_value;
	return (*this);
}

std::vector<Window::DisplayMode> Window::Get_Supported_Display_Modes() {
	std::vector<DisplayMode> displayModes;
	DEVMODE deviceMode;
	DisplayMode displayMode;
	for (uint i = 0; EnumDisplaySettings(NULL, i, &deviceMode) != 0; i++) {
		displayMode.dimensions = Vector2i(deviceMode.dmPelsWidth, deviceMode.dmPelsHeight);
		displayMode.colorBits = deviceMode.dmBitsPerPel;
		displayMode.frequency = deviceMode.dmDisplayFrequency;
		displayModes.push_back(displayMode);
	}
	return displayModes;
}

void Window::_Draw_To_This() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, params.dimensions.X(), params.dimensions.Y());
}
