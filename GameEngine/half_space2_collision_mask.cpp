#include "half_space2_collision_mask.h"

template<class T>
HalfSpace2CollisionMask<T>::HalfSpace2CollisionMask(const HalfSpace<T, 2>& in_halfSpace, bool in_ignoreTransform) :
	CollisionMask<T, 2>(in_ignoreTransform),
	halfSpace(in_halfSpace)
{}

template<class T>
HalfSpace<T, 2>& HalfSpace2CollisionMask<T>::Get_Half_Space() {
	return halfSpace;
}

template<class T>
HalfSpace<T, 2> HalfSpace2CollisionMask<T>::Get_Transformed_Half_Space() const {
	HalfSpace<T, 2> out = halfSpace;
	if (!ignoreTransform) out.Apply_Transform(transform);
	return out;
}

template<class T>
void HalfSpace2CollisionMask<T>::Apply_Transform() {
	halfSpace.Apply_Transform(transform);
}

template<class T>
HalfSpace2CollisionMask<T>* HalfSpace2CollisionMask<T>::Clone() const {
	return new HalfSpace2CollisionMask<T>(*this);
}

template<class T>
Vector<T, 2> HalfSpace2CollisionMask<T>::Get_Closest_Point(const Vector<T, 2>& in_point) const {
	auto transformedHalfSpace = Get_Transformed_Half_Space();
	return in_point.Projection(transformedHalfSpace.Get_Direction().Orthogonal()) + transformedHalfSpace.Get_Point();
}

template<class T>
Vector<T, 2> HalfSpace2CollisionMask<T>::Get_Closest_Normal(const Vector<T, 2>& in_point, PointNormalPolicy in_policy) const {
	return -Get_Transformed_Half_Space().Get_Direction();
}

template<class T>
Collision<T, 2> HalfSpace2CollisionMask<T>::Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

template<class T>
Collision<T, 2> HalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedHalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> HalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedLine2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> HalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedRectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> HalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> HalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> HalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> HalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> HalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Mesh2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> HalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> HalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> HalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> HalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template class HalfSpace2CollisionMask<float>;
template class HalfSpace2CollisionMask<double>;