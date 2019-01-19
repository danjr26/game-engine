#ifndef POINT2_COLLISION_MASK_H
#define POINT2_COLLISION_MASK_H

#include "collision_mask.h"

template<class T>
class Point2CollisionMask : public CollisionMask<T, 2> {
private:
	Vector<T, 2> point;

public:
	Point2CollisionMask(const Vector<T, 2>& in_point, bool in_ignoreTransform = false);

	Vector<T, 2>& Get_Point();
	Vector<T, 2> Get_Transformed_Point() const;
	void Apply_Transform() override;
	Point2CollisionMask* Clone() const override;
	Vector<T, 2> Get_Closest_Point(const Vector<T, 2>& in_point) const override;
	Vector<T, 2>  Get_Closest_Normal(const Vector<T, 2>& in_point, PointNormalPolicy in_policy = towards_point) const override;

	Collision<T, 2> Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedHalfSpace2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedLine2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedRectangleCollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Mesh2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) override;
};

#endif