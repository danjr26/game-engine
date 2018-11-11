#include "axis_aligned_rectangle_collision_mask.h"

template<class T>
AxisAlignedRectangleCollisionMask<T>::AxisAlignedRectangleCollisionMask(const AxisAlignedRectangle<T>& in_rectangle, bool in_ignoreTransform) :
	CollisionMask<T, 2>(in_ignoreTransform),
	rectangle(in_rectangle)
{}

template<class T>
AxisAlignedRectangle<T>& AxisAlignedRectangleCollisionMask<T>::Get_Rectangle() {
	return rectangle;
}

template<class T>
AxisAlignedRectangle<T> AxisAlignedRectangleCollisionMask<T>::Get_Transformed_Rectangle() {
	AxisAlignedRectangle<T> out = rectangle;
	if(!ignoreTransform) out.Apply_Transform(transform);
	return out;
}

template<class T>
void AxisAlignedRectangleCollisionMask<T>::Apply_Transform() {
	rectangle.Apply_Transform(transform);
}

template<class T>
AxisAlignedRectangleCollisionMask<T>* AxisAlignedRectangleCollisionMask<T>::Clone() const {
	return new AxisAlignedRectangleCollisionMask<T>(*this);
}

template<class T>
Collision<T, 2> AxisAlignedRectangleCollisionMask<T>::Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

template<class T>
Collision<T, 2> AxisAlignedRectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedHalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> AxisAlignedRectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedLine2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> AxisAlignedRectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedRectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedRectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedRectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedRectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedRectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedRectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedRectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedRectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedRectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template class AxisAlignedRectangleCollisionMask<float>;
template class AxisAlignedRectangleCollisionMask<double>;