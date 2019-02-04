#include "circle_collision_mask.h"

template<class T>
CircleCollisionMask<T>::CircleCollisionMask(const Circle<T>& in_circle, bool in_ignoreTransform) :
	CollisionMask<T, 2>(in_ignoreTransform),
	circle(in_circle) 
{}

template<class T>
Circle<T>& CircleCollisionMask<T>::Get_Circle() {
	return circle;
}

template<class T>
Circle<T> CircleCollisionMask<T>::Get_Transformed_Circle() const {
	Circle<T> out = circle;
	if (!ignoreTransform) out.Apply_Transform(transform);
	return out;
}

template<class T>
void CircleCollisionMask<T>::Apply_Transform() {
	circle.Apply_Transform(transform);
}

template<class T>
CircleCollisionMask<T>* CircleCollisionMask<T>::Clone() const {
	return new CircleCollisionMask<T>(*this);
}

template<class T>
Vector<T, 2> CircleCollisionMask<T>::Get_Closest_Point(const Vector<T, 2>& in_point) const {
	Circle<T> transformedCircle = Get_Transformed_Circle();
	return (in_point - transformedCircle.Get_Center()).Normalized() * transformedCircle.Get_Radius();
}

template<class T>
Vector<T, 2> CircleCollisionMask<T>::Get_Closest_Normal(const Vector<T, 2>& in_point, PointNormalPolicy in_policy) const {
	Circle<T> transformedCircle = Get_Transformed_Circle();
	return (in_point - transformedCircle.Get_Center()).Normalized();
}

template<class T>
Collision<T, 2> CircleCollisionMask<T>::Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

template<class T>
Collision<T, 2> CircleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AAHalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> CircleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AALine2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> CircleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AARectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> CircleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> CircleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> CircleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> CircleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> CircleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, MST2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> CircleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> CircleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> CircleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> CircleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template class CircleCollisionMask<float>;
template class CircleCollisionMask<double>;

