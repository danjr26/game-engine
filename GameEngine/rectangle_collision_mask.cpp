#include "rectangle_collision_mask.h"
#include "range.h"

template<class T>
RectangleCollisionMask<T>::RectangleCollisionMask(const Box<T, 2>& in_rectangle, bool in_ignoreTransform) :
	CollisionMask<T, 2>(in_ignoreTransform),
	rectangle(in_rectangle)
{}

template<class T>
Box<T, 2>& RectangleCollisionMask<T>::Get_Rectangle() {
	return rectangle;
}

template<class T>
Box<T, 2> RectangleCollisionMask<T>::Get_Transformed_Rectangle() const {
	Box<T, 2> out = rectangle;
	if (!ignoreTransform) out.Apply_Transform(transform);
	return out;
}

template<class T>
void RectangleCollisionMask<T>::Apply_Transform() {
	rectangle.Apply_Transform(transform);
}

template<class T>
RectangleCollisionMask<T>* RectangleCollisionMask<T>::Clone() const {
	return new RectangleCollisionMask<T>(*this);
}

template<class T>
Vector<T, 2> RectangleCollisionMask<T>::Get_Closest_Point(const Vector<T, 2>& in_point) const {
	throw NotImplementedException();
	return Vector<T, 2>();
}

template<class T>
Vector<T, 2> RectangleCollisionMask<T>::Get_Closest_Normal(const Vector<T, 2>& in_point) const {
	auto transformedRectangle = Get_Transformed_Rectangle();
	Vector<T, 2> corners[4];
	Vector<T, 2> axes[2];
	transformedRectangle.Get_Axes(axes);
	transformedRectangle.Get_Corners(corners);
	Vector<T, 2> center = transformedRectangle.Get_Center();
	Vector<T, 2> offset = in_point - center;

	Range<T> range1(axes[0].Dot(corners[0]), axes[0].Dot(corners[3]));
	Range<T> range2(axes[1].Dot(corners[0]), axes[1].Dot(corners[3]));

	T pointDot1 = axes[0].Dot(in_point);
	T pointDot2 = axes[1].Dot(in_point);

	T offsetDot1 = axes[0].Dot(offset);
	T offsetDot2 = axes[1].Dot(offset);

	if (range1.Contains_Inc(pointDot1)) {
		if (range2.Contains_Inc(pointDot2)) {
			return (abs(offsetDot1) > abs(offsetDot2)) ?
				axes[0] * Sign(offsetDot1) :
				axes[1] * Sign(offsetDot2);
		}
		else {
			return axes[0] * Sign(offsetDot1);
		}
	}
	else {
		if (range2.Contains_Inc(pointDot2)) {
			return axes[1] * Sign(offsetDot2);
		}
		else {
			return (abs(offsetDot1) > abs(offsetDot2)) ?
				axes[0] * Sign(offsetDot1) :
				axes[1] * Sign(offsetDot2);
		}
	}
}

template<class T>
Collision<T, 2> RectangleCollisionMask<T>::Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

template<class T>
Collision<T, 2> RectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedHalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> RectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedLine2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> RectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedRectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> RectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> RectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> RectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> RectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> RectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> RectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> RectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> RectangleCollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(*this, in_other);
}

template class RectangleCollisionMask<float>;
template class RectangleCollisionMask<double>;
