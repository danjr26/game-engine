#include "basic_collider.h"
#include "collision_evaluator.h"
#include "log.h"

template<class Basis, class T>
BasicCollider<Basis, T, 2>::BasicCollider(const basis_t& i_basis, bool i_ignoreTransform) :
	Collider<T, 2>(i_ignoreTransform),
	mWrappedBasis{ i_basis }
{}

template<class Basis, class T>
BasicCollider<Basis, T, 2>::~BasicCollider() 
{}

template<class Basis, class T>
Basis& BasicCollider<Basis, T, 2>::getBasis() {
	return mWrappedBasis.mBasis;
}

template<class Basis, class T>
const Basis& BasicCollider<Basis, T, 2>::getBasis() const {
	return mWrappedBasis.mBasis;
}

template<class Basis, class T>
neuterable_ptr<Basis> BasicCollider<Basis, T, 2>::getTransformedBasis() {
	if (mIgnoreTransform) {
		neuterable_ptr_t out = neuterable_ptr_t(&mWrappedBasis.mBasis);
		out.get_deleter().mNeuter = true;
		return out;
	}
	else {
		basis_t* ptr = new basis_t(mWrappedBasis.mBasis);
		neuterable_ptr_t out = neuterable_ptr_t(ptr);
		reinterpret_cast<CollisionBasisWrapper<Basis, T>*>(ptr)->applyTransform(getTransform());
		return out;
	}
}

template<class Basis, class T>
neuterable_ptr<const Basis> BasicCollider<Basis, T, 2>::getTransformedBasis() const {
	if (mIgnoreTransform) {
		neuterable_cptr_t out = neuterable_cptr_t(&mWrappedBasis.mBasis);
		out.get_deleter().mNeuter = true;
		return out;
	}
	else {
		basis_t* ptr = new basis_t(mWrappedBasis.mBasis);
		neuterable_cptr_t out = neuterable_cptr_t(ptr);
		reinterpret_cast<CollisionBasisWrapper<Basis, T>*>(ptr)->applyTransform(getTransform());
		return out;
	}
}

template<class Basis, class T>
void BasicCollider<Basis, T, 2>::applyTransform() {
	mWrappedBasis.applyTransform(getTransform());
}

template<class Basis, class T>
BasicCollider<Basis, T, 2>* BasicCollider<Basis, T, 2>::clone() const {
	return new BasicCollider<Basis, T, 2>(*this);
}

template<class Basis, class T>
Vector<T, 2> BasicCollider<Basis, T, 2>::getClosestPoint(const Vector<T, 2>& i_point) const {
	neuterable_cptr_t transformedBasis = getTransformedBasis();
	return CollisionBasisWrapper<basis_t, T>::getClosestPoint(*transformedBasis, i_point);
}

template<class Basis, class T>
Vector<T, 2> BasicCollider<Basis, T, 2>::getClosestNormal(const Vector<T, 2>& i_point, pnp_t i_policy) const {
	neuterable_cptr_t transformedBasis = getTransformedBasis();
	return CollisionBasisWrapper<basis_t, T>::getClosestNormal(*transformedBasis, i_point, i_policy);
}

template<class Basis, class T>
Collision<T, 2> BasicCollider<Basis, T, 2>::acceptEvaluator1(CollisionEvaluator<T, 2>& i_evaluator, Collider<T, 2>& i_other) {
	return i_other.acceptEvaluator2(i_evaluator, *this);
}

template<class Basis, class T>
Collision<T, 2> BasicCollider<Basis, T, 2>::acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, AAHalfSpace2Collider<T>& i_other) {
	return i_evaluator.evaluateTyped(*this, i_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollider<Basis, T, 2>::acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, AALine2Collider<T>& i_other) {
	return i_evaluator.evaluateTyped(*this, i_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollider<Basis, T, 2>::acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, AARectangleCollider<T>& i_other) {
	return i_evaluator.evaluateTyped(*this, i_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollider<Basis, T, 2>::acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, CircleCollider<T>& i_other) {
	return i_evaluator.evaluateTyped(*this, i_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollider<Basis, T, 2>::acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, HalfSpace2Collider<T>& i_other) {
	return i_evaluator.evaluateTyped(*this, i_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollider<Basis, T, 2>::acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, Line2Collider<T>& i_other) {
	return i_evaluator.evaluateTyped(*this, i_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollider<Basis, T, 2>::acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, LineSegment2Collider<T>& i_other) {
	return i_evaluator.evaluateTyped(*this, i_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollider<Basis, T, 2>::acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, MST2Collider<T>& i_other) {
	return i_evaluator.evaluateTyped(*this, i_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollider<Basis, T, 2>::acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, Point2Collider<T>& i_other) {
	return i_evaluator.evaluateTyped(*this, i_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollider<Basis, T, 2>::acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, Ray2Collider<T>& i_other) {
	return i_evaluator.evaluateTyped(*this, i_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollider<Basis, T, 2>::acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, RectangleCollider<T>& i_other) {
	return i_evaluator.evaluateTyped(*this, i_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollider<Basis, T, 2>::acceptEvaluator2(CollisionEvaluator<T, 2>& i_evaluator, Triangle2Collider<T>& i_other) {
	return i_evaluator.evaluateTyped(*this, i_other);
}

template class BasicCollider<AxisAlignedHalfSpace2d, double, 2>;
template class BasicCollider<AxisAlignedLine2d, double, 2>;
template class BasicCollider<AxisAlignedRectangled, double, 2>;
template class BasicCollider<Circled, double, 2>;
template class BasicCollider<HalfSpace2d, double, 2>;
template class BasicCollider<Line2d, double, 2>;
template class BasicCollider<LineSegment2d, double, 2>;
template class BasicCollider<MeshSphereTree2d, double, 2>;
template class BasicCollider<Vector2d, double, 2>;
template class BasicCollider<Ray2d, double, 2>;
template class BasicCollider<Rectangled, double, 2>;
template class BasicCollider<Triangle2d, double, 2>;

template class BasicCollider<AxisAlignedHalfSpace2f, float, 2>;
template class BasicCollider<AxisAlignedLine2f, float, 2>;
template class BasicCollider<AxisAlignedRectanglef, float, 2>;
template class BasicCollider<Circlef, float, 2>;
template class BasicCollider<HalfSpace2f, float, 2>;
template class BasicCollider<Line2f, float, 2>;
template class BasicCollider<LineSegment2f, float, 2>;
template class BasicCollider<MeshSphereTree2f, float, 2>;
template class BasicCollider<Vector2f, float, 2>;
template class BasicCollider<Ray2f, float, 2>;
template class BasicCollider<Rectanglef, float, 2>;
template class BasicCollider<Triangle2f, float, 2>;