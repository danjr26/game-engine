#ifndef CIRCLE_COLLISION_MASK_H
#define CIRCLE_COLLISION_MASK_H

#include "collision_mask.h"
#include "sphere.h"

// WARNING: does not behave as an ellipse upon non-uniform scaling
class CircleCollisionMask : public CollisionMask2 {
private:
	Circled circle;

public:
	CircleCollisionMask(const Circled& in_circle, bool in_ignoreTransform = false);

	Circled& Get_Circle();
	Circled Get_Transformed_Circle();

	Collision2d Accept_Evaluator(CollisionEvaluator2* in_evaluator, CollisionMask2* in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, AxisAlignedRectangleCollisionMask* in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, CircleCollisionMask* in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, HalfSpace2CollisionMask* in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, Line2CollisionMask* in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, LineSegment2CollisionMask* in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, Point2CollisionMask* in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, Ray2CollisionMask* in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, RectangleCollisionMask* in_other) override;
	Collision2d Accept_Secondhand_Evaluator(CollisionEvaluator2* in_evaluator, Triangle2CollisionMask* in_other) override;
};

#endif