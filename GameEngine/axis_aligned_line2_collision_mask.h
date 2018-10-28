#ifndef AXIS_ALIGNED_LINE2_COLLISION_MASK_H
#define AXIS_ALIGNED_LINE2_COLLISION_MASK_H

#include "collision_mask.h"
#include "axis_aligned_line.h"

class AxisAlignedLine2CollisionMask : public CollisionMask<2> {
private:
	AxisAlignedLine2d line;

public:
	AxisAlignedLine2CollisionMask(const AxisAlignedLine2d& in_line, bool in_ignoreTransform = false);

	AxisAlignedLine2d& Get_Line();
	AxisAlignedLine2d Get_Transformed_Line();
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