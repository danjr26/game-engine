#ifndef TEST_ENEMY_TARGET_H
#define TEST_ENEMY_TARGET_H

#include "ai_target.h"

namespace burning_eye {
	class BurningEye;

	class Target : public AITarget {
	private:
		BurningEye& mParent;

	public:
		Target(BurningEye& in_parent);
		~Target();

		virtual CollisionMask2d const& getCollisionMask() const override;
		virtual Vector2d getLinearVelocity() const override;
		virtual URotation2d getAngularVelocity() const override;
		virtual Allegiance getAllegiance() const override;
	};
}

#endif