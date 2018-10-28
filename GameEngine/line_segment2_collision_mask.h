#ifndef LINE_SEGMENT2_COLLISION_MASK_H
#define LINE_SEGMENT2_COLLISION_MASK_H

#include "collision_mask.h"
#include "line_segment.h"

class LineSegment2CollisionMask : public CollisionMask2 {
private:
	LineSegment2d lineSegment;

public:
	LineSegment2CollisionMask(const LineSegment2d& in_lineSegment, bool in_ignoreTransform = false);

	LineSegment2d& Get_Line_Segment();
	LineSegment2d Get_Transformed_Line_Segment();
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