#include "collision_evaluator.h"
#include "collision_mask.h"

Collisionf CollisionEvaluator::Evaluate(CollisionMask* in_mask1, CollisionMask* in_mask2) {
	return in_mask1->Accept_Evaluator(this, in_mask2);
}
