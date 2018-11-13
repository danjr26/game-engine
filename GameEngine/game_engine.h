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

#define GE GameEngine::Instance()

class GameEngine {
private:
	static GameEngine* instance;

	bool hasBegun;

	Clock clock;
	FrameRateManager frameRateManager;
	PerFrameUpdateManager perFrameUpdateManager;
	InputManager inputManager;
	CameraManager cameraManager;
	AssetManager assetManager;
	FontManager fontManager;
	RenderManager renderManager;
	WindowManager windowManager;
	CollisionManagerd collisionManager;
	AsyncTaskManager asyncTaskManager;
	PhysicsManager physicsManager;

public:
	GameEngine();
	~GameEngine();

	FrameRateManager& Frame_Rate();
	PerFrameUpdateManager& Per_Frame_Update();
	InputManager& Input();
	CameraManager& Cameras();
	AssetManager& Assets();
	FontManager& Fonts();
	RenderManager& Render();
	WindowManager& Windows();
	CollisionManagerd& Collision();
	AsyncTaskManager& Async();
	PhysicsManager& Physics();

	void Begin();

	static GameEngine& Instance();

private:
	void Next_Frame();
};

/*
render
	-window
input
	-mouse
	-keyboard
physics
	-collision
		-raycast
updater
	-frame-synced
	-non-frame-synced
*/

#endif