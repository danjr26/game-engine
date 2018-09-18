#include "collision_evaluator.h"
#include "collision_mask.h"

Collision2d CollisionEvaluator<2>::Evaluate(CollisionMask2* in_mask1, CollisionMask2* in_mask2) {
	return in_mask1->Accept_Evaluator(this, in_mask2);
}
