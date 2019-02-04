#include "basic_collision_mask.h"
#include "collision_evaluator.h"

template<class Basis, class T>
BasicCollisionMask<Basis, T, 2>::BasicCollisionMask(const basis_t& in_basis, bool in_ignoreTransform) :
	CollisionMask<T, 2>(in_ignoreTransform),
	wrappedBasis{ in_basis }
{}

template<class Basis, class T>
Basis& BasicCollisionMask<Basis, T, 2>::Get_Basis() {
	return wrappedBasis.basis;
}

template<class Basis, class T>
const Basis& BasicCollisionMask<Basis, T, 2>::Get_Basis() const {
	return wrappedBasis.basis;
}

template<class Basis, class T>
neuterable_ptr<Basis> BasicCollisionMask<Basis, T, 2>::Get_Transformed_Basis() {
	if (ignoreTransform) {
		neuterable_ptr_t out = neuterable_ptr_t(&wrappedBasis.basis);
		out.get_deleter().neuter = true;
		return out;
	}
	else {
		basis_t* ptr = new basis_t(wrappedBasis.basis);
		neuterable_ptr_t out = neuterable_ptr_t(ptr);
		reinterpret_cast<CollisionBasisWrapper<Basis, T>*>(ptr)->Apply_Transform(transform);
		return out;
	}
}

template<class Basis, class T>
neuterable_ptr<const Basis> BasicCollisionMask<Basis, T, 2>::Get_Transformed_Basis() const {
	if (ignoreTransform) {
		neuterable_cptr_t out = neuterable_cptr_t(&wrappedBasis.basis);
		out.get_deleter().neuter = true;
		return out;
	}
	else {
		basis_t* ptr = new basis_t(wrappedBasis.basis);
		neuterable_cptr_t out = neuterable_cptr_t(ptr);
		reinterpret_cast<CollisionBasisWrapper<Basis, T>*>(ptr)->Apply_Transform(transform);
		return out;
	}
}

template<class Basis, class T>
void BasicCollisionMask<Basis, T, 2>::Apply_Transform() {
	wrappedBasis.Apply_Transform(transform);
}

template<class Basis, class T>
BasicCollisionMask<Basis, T, 2>* BasicCollisionMask<Basis, T, 2>::Clone() const {
	return new BasicCollisionMask<Basis, T, 2>(*this);
}

template<class Basis, class T>
Vector<T, 2> BasicCollisionMask<Basis, T, 2>::Get_Closest_Point(const Vector<T, 2>& in_point) const {
	neuterable_cptr_t transformedBasis = Get_Transformed_Basis();
	return CollisionBasisWrapper<basis_t, T>::Get_Closest_Point(*transformedBasis, in_point);
}

template<class Basis, class T>
Vector<T, 2> BasicCollisionMask<Basis, T, 2>::Get_Closest_Normal(const Vector<T, 2>& in_point, pnp_t in_policy) const {
	neuterable_cptr_t transformedBasis = Get_Transformed_Basis();
	return CollisionBasisWrapper<basis_t, T>::Get_Closest_Normal(*transformedBasis, in_point, in_policy);
}

template<class Basis, class T>
Collision<T, 2> BasicCollisionMask<Basis, T, 2>::Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

template<class Basis, class T>
Collision<T, 2> BasicCollisionMask<Basis, T, 2>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AAHalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollisionMask<Basis, T, 2>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AALine2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollisionMask<Basis, T, 2>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AARectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollisionMask<Basis, T, 2>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollisionMask<Basis, T, 2>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollisionMask<Basis, T, 2>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollisionMask<Basis, T, 2>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollisionMask<Basis, T, 2>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, MST2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollisionMask<Basis, T, 2>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollisionMask<Basis, T, 2>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollisionMask<Basis, T, 2>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class Basis, class T>
Collision<T, 2> BasicCollisionMask<Basis, T, 2>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template class BasicCollisionMask<AxisAlignedHalfSpace2d, double, 2>;
template class BasicCollisionMask<AxisAlignedLine2d, double, 2>;
template class BasicCollisionMask<AxisAlignedRectangled, double, 2>;
template class BasicCollisionMask<Circled, double, 2>;
template class BasicCollisionMask<HalfSpace2d, double, 2>;
template class BasicCollisionMask<Line2d, double, 2>;
template class BasicCollisionMask<LineSegment2d, double, 2>;
//template class BasicCollisionMask<MeshVertexData, double, 2>;
template class BasicCollisionMask<Vector2d, double, 2>;
template class BasicCollisionMask<Ray2d, double, 2>;
template class BasicCollisionMask<Rectangled, double, 2>;
template class BasicCollisionMask<Triangle2d, double, 2>;

template class BasicCollisionMask<AxisAlignedHalfSpace2f, float, 2>;
template class BasicCollisionMask<AxisAlignedLine2f, float, 2>;
template class BasicCollisionMask<AxisAlignedRectanglef, float, 2>;
template class BasicCollisionMask<Circlef, float, 2>;
template class BasicCollisionMask<HalfSpace2f, float, 2>;
template class BasicCollisionMask<Line2f, float, 2>;
template class BasicCollisionMask<LineSegment2f, float, 2>;
//template class BasicCollisionMask<MeshVertexData, float, 2>;
template class BasicCollisionMask<Vector2f, float, 2>;
template class BasicCollisionMask<Ray2f, float, 2>;
template class BasicCollisionMask<Rectanglef, float, 2>;
template class BasicCollisionMask<Triangle2f, float, 2>;