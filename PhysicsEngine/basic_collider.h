#ifndef BASIC_COLLIDER_H
#define BASIC_COLLIDER_H

#include "collider_basis.h"
#include "misc.h"

template<class Basis, class T, uint n>
class BasicCollider
{};


template<class Basis, class T>
class BasicCollider<Basis, T, 2> : public Collider<T, 2> {
public:
	using basis_t = Basis;
	using neuterable_ptr_t = neuterable_ptr<basis_t>;
	using neuterable_cptr_t = neuterable_ptr<const basis_t>;
	using pnp_t = typename Collider<T, 2>::PointNormalPolicy;

private:
	CollisionBasisWrapper<basis_t, T> mWrappedBasis;

public:
	BasicCollider(const basis_t& i_basis, bool i_ignoreTransform = false);
	virtual ~BasicCollider();

	basis_t& getBasis();
	const basis_t& getBasis() const;
	neuterable_ptr_t getTransformedBasis();
	neuterable_cptr_t getTransformedBasis() const;
	void applyTransform() override;
	BasicCollider<Basis, T, 2>* clone() const override;
	Vector<T, 2> getClosestPoint(const Vector<T, 2>& i_point) const override;
	Vector<T, 2> getClosestNormal(const Vector<T, 2>& i_point, pnp_t i_policy = towards_point) const override;

	Collision<T, 2> acceptEvaluator1(CollisionEvaluator<T, 2>& i_evaluator, Collider<T, 2>& i_other) override;
	Collision<T, 2> acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, AAHalfSpace2Collider<T>& i_other) override;
	Collision<T, 2> acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, AALine2Collider<T>& i_other) override;
	Collision<T, 2> acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, AARectangleCollider<T>& i_other) override;
	Collision<T, 2> acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, CircleCollider<T>& i_other) override;
	Collision<T, 2> acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, HalfSpace2Collider<T>& i_other) override;
	Collision<T, 2> acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, Line2Collider<T>& i_other) override;
	Collision<T, 2> acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, LineSegment2Collider<T>& i_other) override;
	Collision<T, 2> acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, MST2Collider<T>& i_other) override;
	Collision<T, 2> acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, Point2Collider<T>& i_other) override;
	Collision<T, 2> acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, Ray2Collider<T>& i_other) override;
	Collision<T, 2> acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, RectangleCollider<T>& i_other) override;
	Collision<T, 2> acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, Triangle2Collider<T>& i_other) override;
};

#endif

