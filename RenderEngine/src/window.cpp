#include "../include/internal/window.h"

const DWORD Window::defaultStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
const DWORD Window::defaultStyleEx = 0;
const Vector2ui Window::defaultDimen = Vector2ui(800, 600);
const std::string Window::defaultName = "Test App";
const Window::PixelFormat Window::defaultFormat = { 32, 24, 8 };

LRESULT CALLBACK dummyWindowProc(HWND i_window, UINT i_message, WPARAM i_wParam, LPARAM i_lParam) {
	return DefWindowProcA(i_window, i_message, i_wParam, i_lParam);
}

Window::Window() :
mWindow(nullptr),
mRenderContext(nullptr)
{}

Window::~Window() {
	if (wglGetCurrentContext() == mRenderContext) {
		if (!wglMakeCurrent(nullptr, nullptr)) fail();
	}
	if (!wglDeleteContext(mRenderContext)) fail();
	if (!DestroyWindow(mWindow)) fail();
}

void Window::flipBuffers() {
	if (!SwapBuffers(GetDC(mWindow))) fail();
}

void Window::init(const PixelFormat& i_format) {
	// create window class
	WNDCLASSEXA windowClass = {};
	windowClass.hInstance = GetModuleHandleA(nullptr);
	windowClass.lpszClassName = (std::string("class") + std::to_string((uintptr_t)this)).c_str();
	windowClass.lpfnWndProc = nullptr;
	windowClass.style = CS_DBLCLKS | CS_OWNDC;
	windowClass.cbSize = sizeof(windowClass);
	windowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	windowClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszMenuName = nullptr;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.lpfnWndProc = dummyWindowProc;
	if (!RegisterClassExA(&windowClass)) fail();

	// create window
	RECT windowBox = { 0, 0, (LONG)defaultDimen.x(), (LONG)defaultDimen.y() };
	if (!AdjustWindowRectEx(&windowBox, defaultStyle, false, defaultStyleEx)) fail();
	mWindow = CreateWindowExA(
		defaultStyleEx,
		windowClass.lpszClassName,
		defaultName.c_str(),
		defaultStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowBox.right - windowBox.left,
		windowBox.bottom - windowBox.top,
		HWND_DESKTOP,
		nullptr,
		GetModuleHandleA(nullptr),
		nullptr
	);
	if (!mWindow) fail();

	// get window device context
	HDC deviceContext = GetDC(mWindow);
	if (!deviceContext) fail();

	// set pixel format
	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
	pfd.iPixelType = PFD_TYPE_RGBA,
		pfd.cColorBits = i_format.mNColorBits;
	pfd.cDepthBits = i_format.mNDepthBits;
	pfd.cStencilBits = i_format.mNStencilBits;
	if (!SetPixelFormat(deviceContext, ChoosePixelFormat(deviceContext, &pfd), &pfd)) fail();

	// create OpenGL render context attached to this window
	mRenderContext = wglCreateContext(deviceContext);
	if (!mRenderContext) fail();

	// init OpenGL context
	wglMakeCurrent(deviceContext, mRenderContext);
	glewInit();
}

void Window::show() {
	ShowWindow(mWindow, SW_SHOWNORMAL);
}

void Window::unshow() {
	ShowWindow(mWindow, SW_HIDE);
}

bool Window::isShown() {
	return IsWindowVisible(mWindow);
}

void Window::maximize() {
	ShowWindow(mWindow, SW_MAXIMIZE);
}

void Window::unmaximize() {
	ShowWindow(mWindow, SW_RESTORE);
}

bool Window::isMaximized() {
	return getStyleBit(WS_MAXIMIZE);
}

void Window::minimize() {
	ShowWindow(mWindow, SW_MINIMIZE);
}

void Window::unminimize() {
	ShowWindow(mWindow, SW_RESTORE);
}

bool Window::isMinimized() {
	return getStyleBit(WS_MINIMIZE);
}

Vector2i Window::getPosition() const {
	RECT rect;
	GetClientRect(mWindow, &rect);
	ClientToScreen(mWindow, (LPPOINT)&rect.left);
	return Vector2i(rect.left, rect.top);
}

void Window::setPosition(const Vector2i& i_position) {
	SetWindowPos(mWindow, nullptr, i_position.x(), i_position.y(), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

Vector2ui Window::getDimen() const {
	RECT rect;
	GetClientRect(mWindow, &rect);
	ClientToScreen(mWindow, (LPPOINT)&rect.left);
	ClientToScreen(mWindow, (LPPOINT)&rect.right);
	return Vector2ui(rect.right - rect.left, rect.bottom - rect.top);
}

void Window::setDimen(const Vector2ui& i_dimen) {
	SetWindowPos(mWindow, nullptr, 0, 0, i_dimen.x(), i_dimen.y(), SWP_NOZORDER | SWP_NOMOVE);
}

std::string Window::getName() const {
	uint nameLength = GetWindowTextLengthA(mWindow);
	if (!nameLength) return std::string();
	char* name = new char[nameLength];
	GetWindowTextA(mWindow, name, nameLength);
	std::string nameString = name;
	delete[] name;
	return nameString;
}

void Window::setName(const std::string& i_name) {
	SetWindowTextA(mWindow, i_name.c_str());
}

void Window::setInputCallback(InputCallback i_callback) {
	SetWindowLongPtrA(mWindow, GWLP_WNDPROC, (LONG_PTR)i_callback);
	SetWindowPos(mWindow, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void Window::setStyleBit(LONG i_bit) {
	SetWindowLongPtrA(mWindow, GWL_STYLE, GetWindowLongPtrA(mWindow, GWL_STYLE) | i_bit);
	SetWindowPos(mWindow, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void Window::clearStyleBit(LONG i_bit) {
	SetWindowLongPtrA(mWindow, GWL_STYLE, GetWindowLongPtrA(mWindow, GWL_STYLE) & ~i_bit);
	SetWindowPos(mWindow, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

bool Window::getStyleBit(LONG i_bit) {
	return GetWindowLongPtrA(mWindow, GWL_STYLE) & i_bit;
}

void Window::setStyleExBit(LONG i_bit) {
	SetWindowLongPtrA(mWindow, GWL_EXSTYLE, GetWindowLongPtrA(mWindow, GWL_EXSTYLE) | i_bit);
	SetWindowPos(mWindow, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void Window::clearStyleExBit(LONG i_bit) {
	SetWindowLongPtrA(mWindow, GWL_EXSTYLE, GetWindowLongPtrA(mWindow, GWL_EXSTYLE) & ~i_bit);
	SetWindowPos(mWindow, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

bool Window::getStyleExBit(LONG i_bit) {
	return GetWindowLongPtrA(mWindow, GWL_EXSTYLE) & i_bit;
}

void Window::_activate() {
	Vector2ui dimen = _getRenderTargetDimen();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, dimen.x(), dimen.y());
}

Vector2ui Window::_getRenderTargetDimen() const {
	return getDimen();
}