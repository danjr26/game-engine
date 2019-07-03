#ifndef TEST_ENEMY_TARGET_H
#define TEST_ENEMY_TARGET_H

#include "ai_target.h"

class BurningEye;

class BurningEyeTarget : public AITarget {
	private:
		BurningEye& mParent;

	public:
		BurningEyeTarget(BurningEye& in_parent);
		~BurningEyeTarget();

		virtual CollisionMask2d const& getCollisionMask() const override;
		virtual Vector2d getLinearVelocity() const override;
		virtual URotation2d getAngularVelocity() const override;
		virtual Allegiance getAllegiance() const override;
};

#endif