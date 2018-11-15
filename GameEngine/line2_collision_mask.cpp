#include "line2_collision_mask.h"

template<class T>
Line2CollisionMask<T>::Line2CollisionMask(const Line<T, 2>& in_line, bool in_ignoreTransform) :
	CollisionMask<T, 2>(ignoreTransform),
	line(in_line)
{}

template<class T>
Line<T, 2>& Line2CollisionMask<T>::Get_Line() {
	return line;
}

template<class T>
Line<T, 2> Line2CollisionMask<T>::Get_Transformed_Line() const {
	Line<T, 2> out = line;
	if (!ignoreTransform) out.Apply_Transform(transform);
	return out;
}

template<class T>
void Line2CollisionMask<T>::Apply_Transform() {
	line.Apply_Transform(transform);
}

template<class T>
Line2CollisionMask<T>* Line2CollisionMask<T>::Clone() const {
	return new Line2CollisionMask<T>(*this);
}

template<class T>
Vector<T, 2> Line2CollisionMask<T>::Get_Closest_Point(const Vector<T, 2>& in_point) const {
	throw NotImplementedException();
	return Vector<T, 2>();
}

template<class T>
Vector<T, 2> Line2CollisionMask<T>::Get_Closest_Normal(const Vector<T, 2>& in_point) const {
	auto transformedLine = Get_Transformed_Line();
	Vector<T, 2> normal = transformedLine.Get_Direction().Orthogonal();
	return (normal.Dot(in_point) >= normal.Dot(transformedLine.Get_Point())) ? normal : -normal;
}

template<class T>
Collision<T, 2> Line2CollisionMask<T>::Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

template<class T>
Collision<T, 2> Line2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedHalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Line2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedLine2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Line2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedRectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Line2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Line2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Line2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Line2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> Line2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> Line2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> Line2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> Line2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template class Line2CollisionMask<float>;
template class Line2CollisionMask<double>;