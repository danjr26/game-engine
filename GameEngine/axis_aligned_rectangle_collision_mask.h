#ifndef AXIS_ALIGNED_RECTANGLE_COLLISION_MASK_H
#define AXIS_ALIGNED_RECTANGLE_COLLISION_MASK_H

#include "collision_mask.h"
#include "axis_aligned_box.h"
#include "range.h"

template<class T>
class AxisAlignedRectangleCollisionMask : public CollisionMask<T, 2> {
private:
	AxisAlignedRectangle<T> rectangle;

public:
	AxisAlignedRectangleCollisionMask(const AxisAlignedRectangle<T>& in_rectangle, bool in_ignoreTransform = false);

	AxisAlignedRectangle<T>& Get_Rectangle();
	AxisAlignedRectangle<T> Get_Transformed_Rectangle() const;
	void Apply_Transform() override;
	AxisAlignedRectangleCollisionMask<T>* Clone() const override;
	Vector<T, 2> Get_Closest_Point(const Vector<T, 2>& in_point) const override;
	Vector<T, 2> Get_Closest_Normal(const Vector<T, 2>& in_point) const override;
			
	Collision<T, 2> Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedHalfSpace2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedLine2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedRectangleCollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) override;
};

#endif