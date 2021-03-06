#include "game_engine.h"
#include "game.h"

GameEngine* GameEngine::instance = nullptr;

GameEngine::GameEngine() :
mHasBegun(false),
mAsyncTaskManager(mClock),
mFrameRateManager(60),
mGame(nullptr) {
	if (instance != nullptr) {
		throw ProcessFailureException("game engine already created");
	}
	else {
		instance = this;
	}
}

GameEngine::~GameEngine() {
	instance = nullptr;
	delete mGame;
}

FrameRateManager& GameEngine::frameRate() {
	return mFrameRateManager;
}

PerFrameUpdateManager& GameEngine::perFrameUpdate() {
	return mPerFrameUpdateManager;
}

InputManager& GameEngine::input() {
	return mInputManager;
}

CameraManager& GameEngine::cameras() {
	return mCameraManager;
}

AssetManager& GameEngine::assets() {
	return mAssetManager;
}

FontManager& GameEngine::fonts() {
	return mFontManager;
}

RenderManager& GameEngine::render() {
	return mRenderManager;
}

WindowManager& GameEngine::windows() {
	return mWindowManager;
}

CollisionManagerd& GameEngine::collision() {
	return mCollisionManager;
}

AsyncTaskManager& GameEngine::async() {
	return mAsyncTaskManager;
}

PhysicsManager& GameEngine::physics() {
	return mPhysicsManager;
}

DestructionManager& GameEngine::destruction() {
	return mDestructionManager;
}

Clock& GameEngine::clock() {
    return mClock;
}

Game& GameEngine::game() {
	return *mGame;
}

void GameEngine::begin() {
	try {
		mGame = new Game();
		mGame->init();

		if (mHasBegun) {
			throw ProcessFailureException("game engine already started");
		}
		mHasBegun = true;

		mFrameRateManager.resetTimer();

		for (uint i = 0; true; i++) {
			nextFrame();
			mFrameRateManager.yieldUntilNextFrame();
		}
	}
	catch (const std::exception& e) {
		handleFatalException(e);
	}
	catch (...) {
		handleFatalException();
	}
}

void GameEngine::quit() {
	delete this;
	exit(0);
}

GameEngine& GameEngine::getInstance() {
	return *instance;
}

bool GameEngine::exists() {
	return instance != nullptr;
}

void GameEngine::nextFrame() {
	static uint count = 0;
	static uint cCount = 0;
	static uint rCount = 0;
	static double t = 0.0;

	cCount++;
	double now = GE.clock().now();
	if (now - t >= 1.0) {
		count++;
		Log::main(std::string("second ") + std::to_string(count) + ":");
		Log::main(std::string(" cSPF (ms): ") + std::to_string((now - t) / cCount * 1000.0));
		Log::main(std::string(" rSPF (ms): ") + std::to_string((now - t) / rCount * 1000.0));
		t = now;
		cCount = 0;
		rCount = 0;
	}

	double dt = mFrameRateManager.getDt();
	double t1 = GE.clock().now();
	mInputManager.update();
	mCollisionManager.update();
	mPhysicsManager.update(dt);
	mGame->preupdate(dt);
	mPerFrameUpdateManager.update(dt);
	mGame->postupdate(dt);
	double t2 = GE.clock().now();
	if (!mFrameRateManager.isLeanFrame() || cCount % 10 == 0) {
		rCount++;
		mRenderManager.mMainWindow->flipBuffers();
		mRenderManager.renderFrame();
	}
	mDestructionManager.update();
	double t3 = GE.clock().now();
}
