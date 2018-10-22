#ifndef RAY2_COLLISION_MASK_H
#define RAY2_COLLISION_MASK_H

#include "collision_mask.h"
#include "ray.h"

class Ray2CollisionMask : public CollisionMask2 {
private:
	Ray2d ray;

public:
	Ray2CollisionMask(const Ray2d& in_ray, bool in_ignoreTransform = false);

	Ray2d& Get_Ray();
	Ray2d Get_Transformed_Ray();

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