#include "render.h"
#include "framework.h"

Window::Window(HINSTANCE hinst, Vector2i dim, std::wstring name) {
	winclass.hInstance =		hinst;
	winclass.lpszClassName =	name.c_str();
	winclass.lpfnWndProc =		WindowProc;
	winclass.style =			CS_DBLCLKS | CS_OWNDC;
	winclass.cbSize =			sizeof(WNDCLASSEX);
	winclass.hIcon =			LoadIcon(nullptr, IDI_APPLICATION);
	winclass.hIconSm =			LoadIcon(nullptr, IDI_APPLICATION);
	winclass.hCursor =			LoadCursor(nullptr, IDC_ARROW);
	winclass.lpszMenuName =		nullptr;
	winclass.cbClsExtra =		0;
	winclass.cbWndExtra =		0;
	winclass.hbrBackground =	(HBRUSH)COLOR_BACKGROUND;

	if (!RegisterClassEx(&winclass)) {
		throw MethodFailureException("Window:Window:RegisterClassEx");
	}

	RECT rect = { 0, 0, dim.x, dim.y };
;
	if (!AdjustWindowRect((LPRECT)&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, false)) {
		throw MethodFailureException("Window:Window:AdjustWindowRect");
	}

	hwnd = CreateWindowEx(
		0,                   
		winclass.lpszClassName,
		winclass.lpszClassName,       	
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,       
		CW_USEDEFAULT,       
		rect.right - rect.left,                
		rect.bottom - rect.top,                 
		HWND_DESKTOP,        
		nullptr,               
		hinst,      
		nullptr 
	);

	hdc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_DOUBLEBUFFER |
		PFD_SUPPORT_OPENGL,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		0,
		PFD_MAIN_PLANE,
		0, 0, 0
	};

	int pixelformat = ChoosePixelFormat(hdc, &pfd);

	SetPixelFormat(hdc, pixelformat, &pfd);

	hglrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglrc);
	
	HGLRC testhglrc = wglGetCurrentContext();

	ShowWindow(hwnd, SW_SHOWNORMAL);
}

Window::~Window() {
	std::lock_guard<std::mutex> lock(mutex);
	DestroyWindow(hwnd);
}

void Window::Flip_Buffers() {
	std::lock_guard<std::mutex> lock(mutex);
	SwapBuffers(hdc);
}

Vector2i Window::Get_Pos() {
	std::lock_guard<std::mutex> lock(mutex);
	RECT rect;
	GetClientRect(hwnd, &rect);
	return Vector2i(rect.left, rect.top);
}

Vector2i Window::Get_Dim() {
	std::lock_guard<std::mutex> lock(mutex);
	RECT rect;
	GetClientRect(hwnd, &rect);
	return Vector2i(rect.right - rect.left, rect.bottom - rect.top);
}

Vector2i Window::Get_Full_Pos() {
	std::lock_guard<std::mutex> lock(mutex);
	RECT rect;
	GetWindowRect(hwnd, &rect);
	return Vector2i(rect.left, rect.top);
}

Vector2i Window::Get_Full_Dim() {
	std::lock_guard<std::mutex> lock(mutex);
	RECT rect;
	GetWindowRect(hwnd, &rect);
	return Vector2i(rect.right - rect.left, rect.bottom - rect.top);
}

void Window::Set_Position(Vector2i pos) {
	std::lock_guard<std::mutex> lock(mutex);
	SetWindowPos(hwnd, 0, pos.x, pos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Window::Set_Dim(Vector2i dim) {
	std::lock_guard<std::mutex> lock(mutex);
	SetWindowPos(hwnd, 0, 0, 0, dim.x, dim.y, SWP_NOMOVE | SWP_NOZORDER);
}

HWND Window::Get_Handle() {
	return hwnd;
}

RenderManager::RenderManager(Vector2i resolution, double step) :
components		(1024, offsetof(class RenderComponent, renderManagerArrayIndex)),
timeRequest		(step),
camera			(Transform3d(), Projection(0.1, 1000.0, 45.0)),
context			(),
lightmanager	(),
overlaymanager	(),
lasermanager	(),
psmanager		(),
textmanager		(),
fb1				(nullptr),
fb2				(nullptr),
resolution		(resolution) {

	glewExperimental = GL_TRUE;

	if (wglGetCurrentContext() == nullptr) {
		throw MethodFailureException("RenderManager:RenderManager:wglGetCurrentContext");
	}

	if (glewInit() != GLEW_OK) {
		throw MethodFailureException("RenderManager:RenderManager:glewInit");
	}

	lasermanager.Init_Arrays();
	psmanager.Init_Arrays();

	Texture2D* fbcoltex1 = new Texture2D("", "fbcol", GL_RGBA16F, 0);
	Texture2D* fbdeptex1 = new Texture2D("", "fbdep", GL_DEPTH_COMPONENT, 0);
	Texture2D* fbcoltex2 = new Texture2D("", "fbcol", GL_RGBA16F, 0);
	Texture2D* fbdeptex2 = new Texture2D("", "fbdep", GL_DEPTH_COMPONENT, 0);

	Color4c white = Color4c(1, 1, 1, 1);
	fbcoltex1->Load_Blank(&white.r, 800, 600, 4);
	fbdeptex1->Load_Blank(&white.r, 800, 600, 1);
	fbcoltex2->Load_Blank(&white.r, 800, 600, 4);
	fbdeptex2->Load_Blank(&white.r, 800, 600, 1);

	fb1 = new Framebuffer(fbcoltex1, fbdeptex1, nullptr);
	fb2 = new Framebuffer(fbcoltex2, fbdeptex2, nullptr);
}

RenderManager::~RenderManager() {
	while(components.Size())
		delete components[0];
}

void RenderManager::Render_Frame() {
	GEngine::Get().Window().Flip_Buffers();

	glClearDepth(1.0f);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < components.Size(); i++) {
		if (components[i]->renderFlags & RenderType::beforecam)
			components[i]->Render(RenderType::beforecam);
	}

	camera.View();
	Flags passes[5] = {
		RenderType::first,
		RenderType::opaque,
		RenderType::shadow,
		RenderType::transparent,
		RenderType::last
	};

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < components.Size(); j++) {
			if (components[j]->renderFlags & passes[i])
				components[j]->Render(passes[i]);
		}
	}
	
	/*glBindFramebuffer(GL_READ_FRAMEBUFFER, fb->fbid);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	fb1->Render_To(fb2);
	fb2->Render_To(fb1);
	fb1->Render_To(nullptr);
	glEnable(GL_DEPTH_TEST);
	/*
	glBlitFramebuffer(0, 0, 800, 600,
		0, 0, 800, 600,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);*/

	//fb1->Apply(FramebufferFlags::write);
}

void RenderManager::Add(RenderComponent* component) {
	components.Add(component);
}

void RenderManager::Remove(RenderComponent* component) {
	components.Remove(component);
}

Camera& RenderManager::Camera() {
	return camera;
}

LightManager& RenderManager::Light() {
	return lightmanager;
}

OverlayManager& RenderManager::Overlay() {
	return overlaymanager;
}

LaserManager& RenderManager::Laser() {
	return lasermanager;
}

ParticleSystemManager& RenderManager::Particle() {
	return psmanager;
}

TextManager& RenderManager::Text() {
	return textmanager;
}

Vector2i RenderManager::Get_Resolution() {
	return resolution;
}

double RenderManager::Get_Target_Frame_Freq() {
	return timeRequest.step;
}

double RenderManager::Get_Target_Frame_Rate() {
	return 1.0 / timeRequest.step;
}

RenderContext::RenderContext() :
material				(),
blendSourceFactor		(GL_ONE),
blendDestinationFactor	(GL_ZERO),
blendOperation			(GL_ADD),
flags					(enable_depth_write)
{}
