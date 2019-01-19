#include "line_segment2_collision_mask.h"
#include "exceptions.h"

template<class T>
LineSegment2CollisionMask<T>::LineSegment2CollisionMask(const LineSegment<T, 2>& in_lineSegment, bool in_ignoreTransform) :
	CollisionMask<T, 2>(in_ignoreTransform),
	lineSegment(in_lineSegment)
{}

template<class T>
LineSegment<T, 2> & LineSegment2CollisionMask<T>::Get_Line_Segment() {
	return lineSegment;
}

template<class T>
LineSegment<T, 2> LineSegment2CollisionMask<T>::Get_Transformed_Line_Segment() const {
	LineSegment<T, 2> out = lineSegment;
	if (!ignoreTransform) out.Apply_Transform(transform);
	return out;
}

template<class T>
void LineSegment2CollisionMask<T>::Apply_Transform() {
	lineSegment.Apply_Transform(transform);
}

template<class T>
LineSegment2CollisionMask<T>* LineSegment2CollisionMask<T>::Clone() const {
	return new LineSegment2CollisionMask<T>(*this);
}

template<class T>
Vector<T, 2> LineSegment2CollisionMask<T>::Get_Closest_Point(const Vector<T, 2>& in_point) const {
	throw NotImplementedException();
	return Vector<T, 2>();
}

template<class T>
Vector<T, 2> LineSegment2CollisionMask<T>::Get_Closest_Normal(const Vector<T, 2>& in_point, PointNormalPolicy in_policy) const {
	auto transformedLineSegment = Get_Transformed_Line_Segment();
	Vector<T, 2> normal = transformedLineSegment.Get_Direction().Orthogonal();
	Vector<T, 2> points[2] = {
		transformedLineSegment.Get_Point1(),
		transformedLineSegment.Get_Point2()
	};
	Vector<T, 2> direction = transformedLineSegment.Get_Direction();

	if (Is_Between_Inc<T>(
		(in_point - points[0]).Dot(direction),
		0, (points[1] - points[0]).Dot(direction)
	)) {
		(normal.Dot(in_point) >= normal.Dot(points[0])) ? normal : -normal;
	}
	switch (in_policy) {
	case PointNormalPolicy::zero:
		return Vector<T, 2>();
		break;
	case PointNormalPolicy::nearest_edge:
		return (normal.Dot(in_point) >= normal.Dot(points[0])) ? normal : -normal;
		break;
	case PointNormalPolicy::towards_point: {
		uint index = Min_Index({
			(in_point - points[0]).Dot_Self(),
			(in_point - points[1]).Dot_Self(),
			});
		return (in_point - points[index]).Normalized();
	}
		break;
	default:
		throw InvalidArgumentException();
	}
}

template<class T>
Collision<T, 2> LineSegment2CollisionMask<T>::Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

template<class T>
Collision<T, 2> LineSegment2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedHalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> LineSegment2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedLine2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> LineSegment2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedRectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> LineSegment2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> LineSegment2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> LineSegment2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> LineSegment2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> LineSegment2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Mesh2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> LineSegment2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> LineSegment2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> LineSegment2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template<class T>
Collision<T, 2> LineSegment2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template class LineSegment2CollisionMask<float>;
template class LineSegment2CollisionMask<double>;