#include "render.h"
#include "framework.h"


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

	Texture2* fbcoltex1 = new Texture2("", "fbcol", GL_RGBA16F, 0);
	Texture2* fbdeptex1 = new Texture2("", "fbdep", GL_DEPTH_COMPONENT, 0);
	Texture2* fbcoltex2 = new Texture2("", "fbcol", GL_RGBA16F, 0);
	Texture2* fbdeptex2 = new Texture2("", "fbdep", GL_DEPTH_COMPONENT, 0);

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
