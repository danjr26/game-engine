#include "axis_aligned_half_space2_collision_mask.h"

template<class T>
AxisAlignedHalfSpace2CollisionMask<T>::AxisAlignedHalfSpace2CollisionMask(const AxisAlignedHalfSpace<T, 2>& in_halfSpace, bool in_ignoreTransform) :
	CollisionMask<T, 2>(in_ignoreTransform),
	halfSpace(in_halfSpace)
{}

template<class T>
AxisAlignedHalfSpace<T, 2>& AxisAlignedHalfSpace2CollisionMask<T>::Get_Half_Space() {
	return halfSpace;
}

template<class T>
AxisAlignedHalfSpace<T, 2> AxisAlignedHalfSpace2CollisionMask<T>::Get_Transformed_Half_Space() const {
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
void AxisAlignedHalfSpace2CollisionMask<T>::Apply_Transform() {
	halfSpace.Apply_Transform(transform);
}

template<class T>
AxisAlignedHalfSpace2CollisionMask<T>* AxisAlignedHalfSpace2CollisionMask<T>::Clone() const {
	return new AxisAlignedHalfSpace2CollisionMask(*this);
}

template<class T>
Vector<T, 2> AxisAlignedHalfSpace2CollisionMask<T>::Get_Closest_Point(const Vector<T, 2>& in_point) const {
	Vector<T, 2> out = in_point;
	auto transformedHalfSpace = Get_Transformed_Half_Space();
	out[transformedHalfSpace.Get_Dimension()] = transformedHalfSpace.Get_Value();
	return out;
}

template<class T>
Vector<T, 2> AxisAlignedHalfSpace2CollisionMask<T>::Get_Closest_Normal(const Vector<T, 2>& in_point) const {
	return -halfSpace.Get_Direction();
}

template<class T>
Collision<T, 2> AxisAlignedHalfSpace2CollisionMask<T>::Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

template<class T>
Collision<T, 2> AxisAlignedHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedHalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> AxisAlignedHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedLine2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedRectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedHalfSpace2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template class AxisAlignedHalfSpace2CollisionMask<float>;
template class AxisAlignedHalfSpace2CollisionMask<double>;