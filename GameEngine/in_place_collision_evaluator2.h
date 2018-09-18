#ifndef IN_PLACE_COLLISION_EVALUATOR2_H
#define IN_PLACE_COLLISION_EVALUATOR2_H

#include "collision_evaluator.h"
#include "axis_aligned_rectangle_collision_mask.h"
#include "circle_collision_mask.h"
#include "point2_collision_mask.h"

class InPlaceCollisionEvaluator2 : public CollisionEvaluator2 {
	Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask* in_mask1, AxisAlignedRectangleCollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(AxisAlignedRectangleCollisionMask* in_mask1, CircleCollisionMask* in_mask2) override;
	Collision2d Evaluate_Typed(CircleCollisionMask* in_mask1, CircleCollisionMask* in_mask2) override;

};

#endif	