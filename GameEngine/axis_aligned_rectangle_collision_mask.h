#ifndef AXIS_ALIGNED_RECTANGLE_COLLISION_MASK_H
#define AXIS_ALIGNED_RECTANGLE_COLLISION_MASK_H

#include "collision_mask.h"
#include "axis_aligned_box.h"
#include "range.h"

class AxisAlignedRectangleCollisionMask : public CollisionMask2 {
private:
	AxisAlignedRectangled rectangle;

public:
	AxisAlignedRectangleCollisionMask(const AxisAlignedRectangled& in_rectangle, bool in_ignoreTransform = false);

	AxisAlignedRectangled& Get_Rectangle();
	AxisAlignedRectangled Get_Transformed_Rectangle();
	void Apply_Transform() override;
			
	Collision2d Accept_Evaluator(CollisionEvaluator2& in_evaluator, CollisionMask2& in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, AxisAlignedHalfSpace2CollisionMask& in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, AxisAlignedLine2CollisionMask& in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, AxisAlignedRectangleCollisionMask& in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, CircleCollisionMask& in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, HalfSpace2CollisionMask& in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Line2CollisionMask& in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, LineSegment2CollisionMask& in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Point2CollisionMask& in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Ray2CollisionMask& in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, RectangleCollisionMask& in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2& in_evaluator, Triangle2CollisionMask& in_other) override;
};

#endif