#include "framework.h"

GEngineActivator::GEngineActivator(GEngine* in_engine) {
	if (GEngine::active != nullptr) {
		MismatchException("GEngineActivator:GEngineActivator:GEngine already exists");
	}
	GEngine::active = in_engine;
	GEngine::active->setupComplete = false;
}

GEngine* GEngine::active = nullptr;

GEngine::GEngine(HINSTANCE in_hInstance) :
activator			(this),
engineClock			(),
playClock			(),
window				(in_hInstance, Vector2i(800, 600), L"Game Engine"),
timeRequestQueue	(),
resourceManager		(),
mechanicsManager	(),
collisionManager	(),
inputManager		(0.01),
aiManager			(),
renderManager		(Vector2i(800, 600), 1.0f / 60.0f),
initializationScript(),
setupComplete		(true) 
{}

GEngine::~GEngine() 
{}

Window&	GEngine::Window() {
	return window;
}

TimeRequestQueue& GEngine::TRQ() {
	return timeRequestQueue;
}

MechanicsManager& GEngine::Mech() {
	return mechanicsManager;
}
RenderManager& GEngine::Render() {
	return renderManager;
}

CollisionManager& GEngine::Collision() {
	return collisionManager;
}

AIManager& GEngine::AI() {
	return aiManager;
}

ResourceManager& GEngine::Resource() {
	return resourceManager;
}

InputManager& GEngine::Input() {
	return inputManager;
}

Clock& GEngine::Engine_Clock() {
	return engineClock;
}

Clock& GEngine::Play_Clock() {
	return playClock;
}

GEngine& GEngine::Get() {
	return *active;
}

bool GEngine::Exists() {
	return active != NULL && active->setupComplete;
}