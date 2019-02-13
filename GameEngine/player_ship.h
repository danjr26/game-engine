#ifndef PLAYER_SHIP_H
#define PLAYER_SHIP_H

#include "player_ship_renderer.h"
#include "player_ship_mover.h"
#include "player_ship_camera_mover.h"
#include "deep_transformable_object.h"
#include "rigid_body.h"

class PlayerShip : public DeepTransformableObject2d {
private:
	PlayerShipRenderer mRenderer;
	PlayerShipMover mMover;
	PlayerShipCameraMover mCameraMover;
	RigidBody<2> mRigidBody;

public:
	PlayerShip();	

	PlayerShipRenderer& getRenderer();
	PlayerShipMover& getMover();
	PlayerShipCameraMover& getCameraMover();
	RigidBody<2>& getRigidBody();
	CollisionMask2d& getCollisionMask();
};

#endif