#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include "ai.h"
#include "asteroid.h"
#include "camera.h"
#include "clock.h"
#include "collision.h"
#include "collision_mask.h"
#include "input.h"
#include "fx.h"
#include "light.h"
#include "mechanics.h"
#include "mesh.h"
#include "mesh_instance.h"
#include "overlay.h"
#include "particle_system.h"
#include "performance.h"
#include "player_ship.h"
#include "projectiles.h"
#include "random.h"
#include "render.h"
#include "resources.h"
#include "scripts.h"
#include "shader.h"
#include "ship_systems.h"
#include "shock_droid.h"
#include "skybox.h"
#include "swarm_droid.h"
#include "text.h"
#include "texture.h"
#include "trq.h"
#include "vectors.h"

#include <windows.h>
#include <gl/glew.h>
#include <gl/wglew.h>

#include <cstdlib>
#include <fstream>
#include <string>
#include <malloc.h>
#include <iomanip>

class GEngine;

class GEngineActivator {
	friend class GEngine;
private:
	GEngineActivator(GEngine* in_engine);
};

class GEngine {
	friend class GEngineActivator;
private:
	static GEngine* active;

	GEngineActivator	activator;
	Clock				engineClock;
	Clock				playClock;
	Window				window;
	TimeRequestQueue	timeRequestQueue;
	ResourceManager		resourceManager;
	MechanicsManager	mechanicsManager;
	CollisionManager	collisionManager;
	InputManager		inputManager;
	AIManager			aiManager;
	RenderManager		renderManager;
	InitScript			initializationScript;

	bool				setupComplete;

public:
	GEngine	(HINSTANCE in_hInstance);
	~GEngine();

	Window&				Window		();
	TimeRequestQueue&	TRQ			();
	MechanicsManager&	Mech		();
	RenderManager&		Render		();
	CollisionManager&	Collision	();
	AIManager&			AI			();
	ResourceManager&	Resource	();
	InputManager&		Input		();
	Clock&				Engine_Clock();
	Clock&				Play_Clock	();

	static GEngine& Get();
	static bool Exists();
 };


#endif
