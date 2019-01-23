#include "game_engine.h"

GameEngine* GameEngine::instance = nullptr;

GameEngine::GameEngine() :
hasBegun(false),
asyncTaskManager(clock) {
	if (instance != nullptr) {
		throw ProcessFailureException("game engine already created");
	}
	else {
		instance = this;
	}
}

GameEngine::~GameEngine() {
	instance = nullptr;
}

FrameRateManager& GameEngine::Frame_Rate() {
	return frameRateManager;
}

PerFrameUpdateManager& GameEngine::Per_Frame_Update() {
	return perFrameUpdateManager;
}

InputManager& GameEngine::Input() {
	return inputManager;
}

CameraManager& GameEngine::Cameras() {
	return cameraManager;
}

AssetManager& GameEngine::Assets() {
	return assetManager;
}

FontManager& GameEngine::Fonts() {
	return fontManager;
}

RenderManager& GameEngine::Render() {
	return renderManager;
}

WindowManager& GameEngine::Windows() {
	return windowManager;
}

CollisionManagerd& GameEngine::Collision() {
	return collisionManager;
}

AsyncTaskManager& GameEngine::Async() {
	return asyncTaskManager;
}

PhysicsManager& GameEngine::Physics() {
	return physicsManager;
}

Clock& GameEngine::Time() {
    return clock;
}

void GameEngine::Begin() {
	try {
		if (hasBegun) {
			throw ProcessFailureException("game engine already started");
		}
		hasBegun = true;

		frameRateManager.Reset_Timer();

		for (uint i = 0; true; i++) {
			Next_Frame();
			frameRateManager.Yield_Until_Next_Frame();
		}
	}
	catch (const std::exception& e) {
		Handle_Fatal_Exception(e);
	}
	catch (...) {
		Handle_Fatal_Exception();
	}
}

void GameEngine::Exit() {
	delete this;
	exit(0);
}

GameEngine& GameEngine::Instance() {
	return *instance;
}

void GameEngine::Next_Frame() {
	static uint cCount = 0;
	static uint rCount = 0;
	static double t = 0.0;

	cCount++;
	double now = GE.Time().Now();
	if (now - t >= 1.0) {
		Log::main(std::string("cSPF (ms): ") + std::to_string((now - t) / cCount * 1000.0));
		Log::main(std::string("rSPF (ms): ") + std::to_string((now - t) / rCount * 1000.0));
		t = now;
		cCount = 0;
		rCount = 0;
	}

	double dt = frameRateManager.Get_Dt();
	double t1 = GE.Time().Now();
	inputManager.Update();
	collisionManager.Update();
	physicsManager.Update(dt);
	perFrameUpdateManager.Update(dt);
	double t2 = GE.Time().Now();
	if (!frameRateManager.Is_Lean_Frame()) {
		//glFinish();
		rCount++;
		renderManager.mainWindow->Flip_Buffers();
		renderManager.Render_Frame();
	}
	else {
		Log::main("Lean Frame");
	}
	double t3 = GE.Time().Now();

	//Log::main(std::string("Calc: ") + std::to_string((t2 - t1) * 1000.0));
	//Log::main(std::string("Rend: ") + std::to_string((t3 - t2) * 1000.0));
}
