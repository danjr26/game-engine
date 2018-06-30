#include "game_engine.h"

GameEngine* GameEngine::instance = nullptr;

GameEngine::GameEngine() :
hasBegun(false) {
	if (instance != nullptr) {
		Log::main("error: multiple instances of engine");
		exit(-1);
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

void GameEngine::Begin() {
	if (hasBegun) {
		Log::main("error: engine already started");
		exit(-1);
	}
	hasBegun = true;

	frameRateManager.Reset_Timer();

	for(uint i = 0; i < 6000; i++) {
		Next_Frame();
		frameRateManager.Yield_Until_Next_Frame();
		if (i % 60 == 0) {
			Log::main(to_string(frameRateManager.Get_Real_FPS()));
		}
	}
}

GameEngine& GameEngine::Instance() {
	return *instance;
}

void GameEngine::Next_Frame() {
	inputManager.Update();
	perFrameUpdateManager.Update(frameRateManager.Get_Last_Frame_Time());
	renderManager.Render_Frame();
	renderManager.mainWindow->Flip_Buffers();
}