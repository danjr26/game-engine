#include "point2_collision_mask.h"

template<class T>
Point2CollisionMask<T>::Point2CollisionMask(const Vector<T, 2>& in_point, bool in_ignoreTransform) :
	CollisionMask<T, 2>(in_ignoreTransform),
	point(in_point)
{}

template<class T>
Vector<T, 2>& Point2CollisionMask<T>::Get_Point() {
	return point;
}

template<class T>
Vector<T, 2> Point2CollisionMask<T>::Get_Transformed_Point() const {
	Vector<T, 2> out = point;
	if (!ignoreTransform) out = transform.Local_To_World_Point(out);
	return out;
}

template<class T>
void Point2CollisionMask<T>::Apply_Transform() {
	point = transform.Local_To_World_Point(point);
}

template<class T>
Point2CollisionMask<T>* Point2CollisionMask<T>::Clone() const {
	return new Point2CollisionMask<T>(*this);
}

template<class T>
Vector<T, 2> Point2CollisionMask<T>::Get_Closest_Point(const Vector<T, 2>& in_point) const {
	throw NotImplementedException();
	return Vector<T, 2>();
}

template<class T>
Vector<T, 2> Point2CollisionMask<T>::Get_Closest_Normal(const Vector<T, 2>& in_point, PointNormalPolicy in_policy) const {
	switch (in_policy) {
	case PointNormalPolicy::zero:
		return Vector<T, 2>();
		break;
	case PointNormalPolicy::nearest_edge:
	case PointNormalPolicy::towards_point:
		return in_point - Get_Transformed_Point();
		break;
	default:
		throw InvalidArgumentException();
	}
}

template<class T>
Collision<T, 2> Point2CollisionMask<T>::Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

template<class T>
Collision<T, 2> Point2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AAHalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Point2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AALine2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Point2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AARectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Point2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Point2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Point2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Point2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Point2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, MST2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Point2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> Point2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> Point2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> Point2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template class Point2CollisionMask<float>;
template class Point2CollisionMask<double>;