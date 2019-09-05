#ifndef TEST_ENEMY_H
#define TEST_ENEMY_H

#include "burning_eye_renderer.h"
#include "burning_eye_health.h"
#include "burning_eye_mover.h"
#include "deep_transformable_object.h"
#include "rigid_body.h"
#include "destructable_object.h"
#include "per_frame_updateable_object.h"
#include "collision_queue.h"
#include "damage_receiver.h"
#include "burning_eye_controller.h"
#include "burning_eye_target.h"

namespace burning_eye {
	class BurningEye : 
		public DeepTransformableObject2d, 
		public DestructableObject,
		public PerFrameUpdateableObject {
	private:
		RigidBody<2> mRigidBody;
		Renderer mRenderer;
		Mover mMover;
		Health mHealth;
		Controller mController;
		Target mTarget;
		CollisionQueue2d mCollisionQueue;

	public:
		BurningEye();
		~BurningEye();

		void update(double in_dt) override;

		void setMultiplier(double in_multiplier) override;

		Renderer& getRenderer();
		Mover& getMover();
		Health& getHealth();
		Controller& getController();
		Target& getTarget();
		RigidBody<2>& getRigidBody();
		CollisionMask2d& getCollisionMask();
		CollisionQueue2d& getCollisionQueue();

	private:
		void initMembers();
	};
}

#endif