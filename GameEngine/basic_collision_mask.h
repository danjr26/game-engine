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
	CollisionBasisWrapper<basis_t, T> mWrappedBasis;

public:
	BasicCollisionMask(const basis_t& in_basis, bool in_ignoreTransform = false);
	virtual ~BasicCollisionMask();

	basis_t& getBasis();
	const basis_t& getBasis() const;
	neuterable_ptr_t getTransformedBasis();
	neuterable_cptr_t getTransformedBasis() const;
	void applyTransform() override;
	BasicCollisionMask<Basis, T, 2>* clone() const override;
	Vector<T, 2> getClosestPoint(const Vector<T, 2>& in_point) const override;
	Vector<T, 2> getClosestNormal(const Vector<T, 2>& in_point, pnp_t in_policy = towards_point) const override;

	Collision<T, 2> acceptEvaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) override;
	Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, AAHalfSpace2CollisionMask<T>& in_other) override;
	Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, AALine2CollisionMask<T>& in_other) override;
	Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, AARectangleCollisionMask<T>& in_other) override;
	Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) override;
	Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) override;
	Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) override;
	Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) override;
	Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, MST2CollisionMask<T>& in_other) override;
	Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) override;
	Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) override;
	Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) override;
	Collision<T, 2> acceptSecondhandEvaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) override;
};

#endif

