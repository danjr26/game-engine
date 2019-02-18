#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "clock.h"
#include "frame_rate_manager.h"
#include "per_frame_update_manager.h"
#include "asset_manager.h"
#include "font_manager.h"
#include "render_manager.h"
#include "camera_manager.h"
#include "input_manager.h"
#include "log.h"
#include "window_manager.h"
#include "collision_manager.h"
#include "misc.h"
#include "exceptions.h"
#include "async_task_manager.h"
#include "physics_manager.h"
#include "game.h"
#include "destruction_manager.h"

#define GE GameEngine::getInstance()

class GameEngine {
private:
	static GameEngine* instance;

	bool mHasBegun;

	Clock mClock;
	FrameRateManager mFrameRateManager;
	PerFrameUpdateManager mPerFrameUpdateManager;
	InputManager mInputManager;
	CameraManager mCameraManager;
	AssetManager mAssetManager;
	FontManager mFontManager;
	RenderManager mRenderManager;
	WindowManager mWindowManager;
	CollisionManagerd mCollisionManager;
	AsyncTaskManager mAsyncTaskManager;
	PhysicsManager mPhysicsManager;
	DestructionManager mDestructionManager;
	Game* mGame;

public:
	GameEngine();
	~GameEngine();

	FrameRateManager& frameRate();
	PerFrameUpdateManager& perFrameUpdate();
	InputManager& input();
	CameraManager& cameras();
	AssetManager& assets();
	FontManager& fonts();
	RenderManager& render();
	WindowManager& windows();
	CollisionManagerd& collision();
	AsyncTaskManager& async();
	PhysicsManager& physics();
	DestructionManager& destruction();
  Clock& clock();
	Game& game();

	void begin();
	void quit();

	static GameEngine& getInstance();

private:
	void nextFrame();
};

/*
render
	-window
input
	-mMouse
	-keyboard
physics
	-collision
		-raycast
updater
	-frame-synced
	-non-frame-synced
*/

#endif