#include "game_engine.h"

GameEngine* GameEngine::instance = nullptr;

GameEngine::GameEngine() :
mHasBegun(false),
mAsyncTaskManager(mClock) {
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
	return mFrameRateManager;
}

PerFrameUpdateManager& GameEngine::Per_Frame_Update() {
	return mPerFrameUpdateManager;
}

InputManager& GameEngine::Input() {
	return mInputManager;
}

CameraManager& GameEngine::Cameras() {
	return mCameraManager;
}

AssetManager& GameEngine::Assets() {
	return mAssetManager;
}

FontManager& GameEngine::Fonts() {
	return mFontManager;
}

RenderManager& GameEngine::Render() {
	return mRenderManager;
}

WindowManager& GameEngine::Windows() {
	return mWindowManager;
}

CollisionManagerd& GameEngine::Collision() {
	return mCollisionManager;
}

AsyncTaskManager& GameEngine::Async() {
	return mAsyncTaskManager;
}

PhysicsManager& GameEngine::Physics() {
	return mPhysicsManager;
}

Clock& GameEngine::Time() {
    return mClock;
}

void GameEngine::Begin() {
	try {
		if (mHasBegun) {
			throw ProcessFailureException("game engine already started");
		}
		mHasBegun = true;

		mFrameRateManager.Reset_Timer();

		for (uint i = 0; true; i++) {
			Next_Frame();
			mFrameRateManager.Yield_Until_Next_Frame();
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
	static uint count = 0;
	static uint cCount = 0;
	static uint rCount = 0;
	static double t = 0.0;

	cCount++;
	double now = GE.Time().Now();
	if (now - t >= 1.0) {
		count++;
		Log::main(std::string("second ") + std::to_string(count) + ":");
		Log::main(std::string(" cSPF (ms): ") + std::to_string((now - t) / cCount * 1000.0));
		Log::main(std::string(" rSPF (ms): ") + std::to_string((now - t) / rCount * 1000.0));
		t = now;
		cCount = 0;
		rCount = 0;
	}

	double dt = mFrameRateManager.Get_Dt();
	double t1 = GE.Time().Now();
	mInputManager.Update();
	mCollisionManager.Update();
	mPhysicsManager.Update(dt);
	mPerFrameUpdateManager.Update(dt);
	double t2 = GE.Time().Now();
	if (!mFrameRateManager.Is_Lean_Frame()) {
		//glFinish();
		rCount++;
		mRenderManager.mMainWindow->Flip_Buffers();
		mRenderManager.Render_Frame();
	}

	double t3 = GE.Time().Now();

	//Log::main(std::string("Calc: ") + std::to_string((t2 - t1) * 1000.0));
	//Log::main(std::string("Rend: ") + std::to_string((t3 - t2) * 1000.0));
}
