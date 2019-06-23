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

class PlayerShip : public DeepTransformableObject2d, public PerFrameUpdateableObject,
	public DestructableObject {
private:
	PlayerShipRenderer mRenderer;
	PlayerShipMover mMover;
	PlayerShipCameraMover mCameraMover;
	PlayerShipWeaponsSystem mWeaponsSystem;
	PlayerShipTarget mTarget;
	PlayerShipHealth mHealth;
	RigidBody<2> mRigidBody;
	CollisionQueue2d mCollisionQueue;

public:
	PlayerShip();	
	~PlayerShip();

	PlayerShipRenderer& getRenderer();
	PlayerShipMover& getMover();
	PlayerShipCameraMover& getCameraMover();
	RigidBody<2>& getRigidBody();
	CollisionMask2d& getCollisionMask();
	PlayerShipWeaponsSystem& getWeaponsSystem();
	PlayerShipTarget& getTarget();
	PlayerShipHealth& getHealth();
	CollisionQueue2d& getCollisionQueue();

	virtual void update(double in_dt) override;

private:
	void initMembers();
};

#endif