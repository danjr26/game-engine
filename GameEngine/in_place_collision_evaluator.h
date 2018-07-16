#ifndef IN_PLACE_COLLISION_EVALUATOR_H
#define IN_PLACE_COLLISION_EVALUATOR_H

#include "collision_evaluator.h"
#include "circle_collision_mask.h"

class InPlaceCollisionEvaluator : public CollisionEvaluator {
	Collisionf Evaluate_Typed(CircleCollisionMask* in_mask1, CircleCollisionMask* in_mask2) override {
		Collisionf collision;
		const Vector2d position1 = Vector2d(in_mask1->Get_Transform().Get_World_Position());
		const Vector2d position2 = Vector2d(in_mask2->Get_Transform().Get_World_Position());
		const double radii = in_mask1->Get_Radius() + in_mask2->Get_Radius();
		const double radiiSquared = radii * radii;
		const Vector2d offset = position2 - position1;
		const double offsetDistanceSquared = offset.Dot_Self();

		collision.didCollide = offsetDistanceSquared <= radiiSquared;
		if (collision.didCollide) {
			collision.collisionPoint2 = position1 + offset / 2.0;
		}
	}
};

#endif