#ifndef COLLISION_EVALUATOR_H
#define COLLISION_EVALUATOR_H

#include "collision.h"

class CollisionMask;
class CircleCollisionMask;

class CollisionEvaluator {
public:
	Collisionf Evaluate(CollisionMask* in_mask1, CollisionMask* in_mask2);
	virtual Collisionf Evaluate_Typed(CircleCollisionMask* in_mask1, CircleCollisionMask* in_mask2) = 0;
};

#endif
