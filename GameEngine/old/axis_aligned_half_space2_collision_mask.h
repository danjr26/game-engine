#ifndef AXIS_ALIGNED_HALF_SPACE2_COLLISION_MASK_H
#define AXIS_ALIGNED_HALF_SPACE2_COLLISION_MASK_H

#include "collision_mask.h"
#include "axis_aligned_half_space.h"
#include <memory>
#include "misc.h"

template<class T>
class AAHalfSpace2CollisionMask : public CollisionMask<T, 2> {
private:
	AxisAlignedHalfSpace<T, 2> halfSpace;

public:
	AAHalfSpace2CollisionMask(const AxisAlignedHalfSpace<T, 2>& in_halfSpace, bool in_ignoreTransform = false);

	AxisAlignedHalfSpace<T, 2>& Get_Half_Space();
	AxisAlignedHalfSpace<T, 2> Get_Transformed_Half_Space() const;
	void Apply_Transform() override;
	AAHalfSpace2CollisionMask<T>* Clone() const override;
	Vector<T, 2> Get_Closest_Point(const Vector<T, 2>& in_point) const override;
	Vector<T, 2>  Get_Closest_Normal(const Vector<T, 2>& in_point, PointNormalPolicy in_policy = towards_point) const override;

	Collision<T, 2> Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AAHalfSpace2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AALine2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AARectangleCollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, MST2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) override;
	Collision<T, 2> Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) override;

};

#endif