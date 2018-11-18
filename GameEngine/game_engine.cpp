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
			if (i % 60 == 0) {
				Log::main(std::to_string(frameRateManager.Get_Real_FPS()).substr(0, 4));
			}
		}
	}
	catch (const std::exception& e) {
		Handle_Fatal_Exception(e);
	}
	catch (...) {
		Handle_Fatal_Exception();
	}
}

GameEngine& GameEngine::Instance() {
	return *instance;
}

void GameEngine::Next_Frame() {
	static int count = 0;
	count++;
	double dt = frameRateManager.Get_Last_Frame_Time();
	inputManager.Update();
	collisionManager.Update();
	//if(count % 15 == 0) physicsManager.Update(dt * 15);
	physicsManager.Update(dt);
	perFrameUpdateManager.Update(dt);
	renderManager.Render_Frame();
	renderManager.mainWindow->Flip_Buffers();
}
