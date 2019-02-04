#include "axis_aligned_half_space2_collision_mask.h"

template<class T>
AAHalfSpace2CollisionMask<T>::AAHalfSpace2CollisionMask(const AxisAlignedHalfSpace<T, 2>& in_halfSpace, bool in_ignoreTransform) :
	CollisionMask<T, 2>(in_ignoreTransform),
	halfSpace(in_halfSpace)
{}

template<class T>
AxisAlignedHalfSpace<T, 2>& AAHalfSpace2CollisionMask<T>::Get_Half_Space() {
	return halfSpace;
}

template<class T>
AxisAlignedHalfSpace<T, 2> AAHalfSpace2CollisionMask<T>::Get_Transformed_Half_Space() const {
	if (ignoreTransform) {
		return halfSpace;
	}
	else {
		AxisAlignedHalfSpace<T, 2> out = halfSpace;
		out.Apply_Transform(transform);
		return out;
	}
}

template<class T>
void AAHalfSpace2CollisionMask<T>::Apply_Transform() {
	halfSpace.Apply_Transform(transform);
}

template<class T>
AAHalfSpace2CollisionMask<T>* AAHalfSpace2CollisionMask<T>::Clone() const {
	return new AAHalfSpace2CollisionMask(*this);
}

template<class T>
Vector<T, 2> AAHalfSpace2CollisionMask<T>::Get_Closest_Point(const Vector<T, 2>& in_point) const {
	Vector<T, 2> out = in_point;
	auto transformedHalfSpace = Get_Transformed_Half_Space();
	out[transformedHalfSpace.Get_Dimension()] = transformedHalfSpace.Get_Value();
	return out;
}

template<class T>
Vector<T, 2> AAHalfSpace2CollisionMask<T>::Get_Closest_Normal(const Vector<T, 2>& in_point, PointNormalPolicy in_policy) const {
	return -halfSpace.Get_Direction();
}

template<class T>
Collision<T, 2> AAHalfSpace2CollisionMask<T>::Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

template<class T>
Collision<T, 2> AAHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AAHalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> AAHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AALine2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AAHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AARectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AAHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AAHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AAHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AAHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AAHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, MST2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AAHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AAHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AAHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AAHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template class AAHalfSpace2CollisionMask<float>;
template class AAHalfSpace2CollisionMask<double>;
