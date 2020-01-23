#ifndef PLAYER_SHIP_H
#define PLAYER_SHIP_H

#include "player_ship_renderer.h"
#include "player_ship_mover.h"
#include "player_ship_camera_mover.h"
#include "deep_transformable_object.h"
#include "player_ship_weapons_system.h"
#include "player_ship_health.h"
#include "per_frame_updateable_object.h"
#include "player_ship_target.h"
#include "rigid_body.h"
#include "collision_queue.h"
#include "destructable_object.h"

namespace player_ship {
	class PlayerShip :
		public DeepTransformableObject2d,
		public PerFrameUpdateableObject,
		public DestructableObject {
	private:
		Renderer mRenderer;
		Mover mMover;
		CameraMover mCameraMover;
		WeaponsSystem mWeaponsSystem;
		Target mTarget;
		Health mHealth;
		RigidBody<2> mRigidBody;
		CollisionQueue2d mCollisionQueue;

	public:
		PlayerShip();
		~PlayerShip();

		Renderer& getRenderer();
		Mover& getMover();
		CameraMover& getCameraMover();
		RigidBody<2>& getRigidBody();
		Collider2d& getCollider();
		WeaponsSystem& getWeaponsSystem();
		Target& getTarget();
		Health& getHealth();
		CollisionQueue2d& getCollisionQueue();

		virtual void update(double i_dt) override;
		
		void setMultiplier(double i_multiplier) override;

	private:
		void initMembers();
	};
}

#endif