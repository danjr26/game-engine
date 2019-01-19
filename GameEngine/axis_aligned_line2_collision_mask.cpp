#include "axis_aligned_line2_collision_mask.h"

template<class T>
AxisAlignedLine2CollisionMask<T>::AxisAlignedLine2CollisionMask(const AxisAlignedLine<T, 2>& in_line, bool in_ignoreTransform) :
	CollisionMask<T, 2>(in_ignoreTransform),
	line(in_line)
{}

template<class T>
AxisAlignedLine<T, 2>& AxisAlignedLine2CollisionMask<T>::Get_Line() {
	return line;
}

template<class T>
AxisAlignedLine<T, 2> AxisAlignedLine2CollisionMask<T>::Get_Transformed_Line() const {
	AxisAlignedLine<T, 2> out = line;
	if (!ignoreTransform) out.Apply_Transform(transform);
	return out;
}

template<class T>
void AxisAlignedLine2CollisionMask<T>::Apply_Transform() {
	line.Apply_Transform(transform);
}

template<class T>
AxisAlignedLine2CollisionMask<T>* AxisAlignedLine2CollisionMask<T>::Clone() const {
	return new AxisAlignedLine2CollisionMask<T>(*this);
}

template<class T>
Vector<T, 2> AxisAlignedLine2CollisionMask<T>::Get_Closest_Point(const Vector<T, 2>& in_point) const {
	Vector<T, 2> out = in_point;
	auto transformedLine = Get_Transformed_Line();
	out[transformedLine.Get_Dimension()] = transformedLine.Get_Value();
	return out;
}

template<class T>
Vector<T, 2> AxisAlignedLine2CollisionMask<T>::Get_Closest_Normal(const Vector<T, 2>& in_point, PointNormalPolicy in_policy) const {
	return line.Get_Direction().Orthogonal();
}

template<class T>
Collision<T, 2> AxisAlignedLine2CollisionMask<T>::Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

template<class T>
Collision<T, 2> AxisAlignedLine2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedHalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> AxisAlignedLine2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedLine2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> AxisAlignedLine2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedRectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedLine2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedLine2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedLine2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedLine2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedLine2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Mesh2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedLine2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedLine2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedLine2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> AxisAlignedLine2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template class AxisAlignedLine2CollisionMask<float>;
template class AxisAlignedLine2CollisionMask<double>;