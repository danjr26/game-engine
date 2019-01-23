#ifndef BASIC_COLLISION_MASK_H
#define BASIC_COLLISION_MASK_H

#include "collision_basis_wrapper.h"
#include "misc.h"


template<class Basis, class T, uint n>
class BasicCollisionMask
{};


template<class Basis, class T>
class BasicCollisionMask<Basis, T, 2> : public CollisionMask<T, 2> {
public:
	using basis_t = Basis;
	using neuterable_ptr_t = neuterable_ptr<basis_t>;
	using neuterable_cptr_t = neuterable_ptr<const basis_t>;
	using pnp_t = typename CollisionMask<T, 2>::PointNormalPolicy;

private:
	CollisionBasisWrapper<basis_t, T> wrappedBasis;

public:
	BasicCollisionMask(const basis_t& in_basis, bool in_ignoreTransform = false);

	basis_t& Get_Basis();
	const basis_t& Get_Basis() const;
	neuterable_ptr_t Get_Transformed_Basis();
	neuterable_cptr_t Get_Transformed_Basis() const;
	void Apply_Transform() override;
	BasicCollisionMask<Basis, T, 2>* Clone() const override;
	Vector<T, 2> Get_Closest_Point(const Vector<T, 2>& in_point) const override;
	Vector<T, 2> Get_Closest_Normal(const Vector<T, 2>& in_point, pnp_t in_policy = towards_point) const override;

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

