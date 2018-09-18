#ifndef TRIANGLE2_COLLISION_MASK_H
#define TRIANGLE2_COLLISION_MASK_H

#include "collision_mask.h"
#include "triangle.h"

class Triangle2CollisionMask : public CollisionMask2 {
private:
	Triangle2d triangle;

public:
	Triangle2CollisionMask(const Triangle2d& in_Triangle);

	Triangle2d& Get_Triangle();
	Triangle2d Get_Transformed_Triangle();

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