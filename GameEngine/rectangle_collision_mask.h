#ifndef RECTANGLE_COLLISION_MASK_H
#define RECTANGLE_COLLISION_MASK_H

#include "collision_mask.h"
#include "box.h"

class RectangleCollisionMask : public CollisionMask2 {
private:
	Rectangled rectangle;

public:
	RectangleCollisionMask(const Rectangled& in_rectangle, bool in_ignoreTransform = false);

	Rectangled& Get_Rectangle();
	Rectangled Get_Transformed_Rectangle();

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