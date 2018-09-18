#ifndef POINT2_COLLISION_MASK_H
#define POINT2_COLLISION_MASK_H

#include "collision_mask.h"

class Point2CollisionMask : public CollisionMask2 {
private:
	Vector2d point;

public:
	Point2CollisionMask(const Vector2d& in_point);

	Vector2d& Get_Point();
	Vector2d Get_Transformed_Point() const;

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