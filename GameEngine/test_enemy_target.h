#ifndef TEST_ENEMY_TARGET_H
#define TEST_ENEMY_TARGET_H

#include "ai_target.h"

class TestEnemy;

class TestEnemyTarget : public AITarget {
	private:
		TestEnemy& mParent;

	public:
		TestEnemyTarget(TestEnemy& in_parent);
		~TestEnemyTarget();

		virtual CollisionMask2d const& getCollisionMask() const override;
		virtual Vector2d getLinearVelocity() const override;
		virtual URotation2d getAngularVelocity() const override;
		virtual Allegiance getAllegiance() const override;
};

#endif