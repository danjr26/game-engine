#ifndef PLAYER_SHIP_H
#define PLAYER_SHIP_H

#include "player_ship_renderer.h"
#include "player_ship_mover.h"
#include "player_ship_camera_mover.h"
#include "deep_transformable_object.h"
#include "player_ship_weapons_system.h"
#include "player_ship_collision_responder.h"
#include "per_frame_updateable_object.h"
#include "rigid_body.h"

class PlayerShip : public DeepTransformableObject2d, public PerFrameUpdateableObject {
private:
	PlayerShipRenderer mRenderer;
	PlayerShipMover mMover;
	PlayerShipCameraMover mCameraMover;
	RigidBody<2> mRigidBody;
	PlayerShipWeaponsSystem mWeaponsSystem;
	PlayerShipCollisionResponder mCollisionResponder;

public:
	PlayerShip();	

	PlayerShipRenderer& getRenderer();
	PlayerShipMover& getMover();
	PlayerShipCameraMover& getCameraMover();
	RigidBody<2>& getRigidBody();
	CollisionMask2d& getCollisionMask();
	PlayerShipWeaponsSystem& getWeaponsSystem();
	PlayerShipCollisionResponder& getCollisionResponder();

	virtual void update(double in_dt) override;
};

#endif