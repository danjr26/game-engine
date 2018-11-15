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
AxisAlignedRectangle<T> AxisAlignedRectangleCollisionMask<T>::Get_Transformed_Rectangle() const {
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
Vector<T, 2> AxisAlignedRectangleCollisionMask<T>::Get_Closest_Point(const Vector<T, 2>& in_point) const {
	AxisAlignedRectangle<T> transformedRectangle = Get_Transformed_Rectangle();
	Vector<T, 2> center = transformedRectangle.Get_Center();
	Vector<T, 2> offset = in_point - center;
	Range<T> xRange(transformedRectangle.Get_Minima().X(), transformedRectangle.Get_Maxima().X());
	Range<T> yRange(transformedRectangle.Get_Minima().Y(), transformedRectangle.Get_Maxima().Y());
	if (xRange.Contains_Inc(in_point.X())) {
		if (yRange.Contains_Inc(in_point.Y())) {
			return Vector<T, 2>(
				(offset.X() > 0) ? transformedRectangle.Get_Maxima().X() : transformedRectangle.Get_Minima().X(),
				(offset.Y() > 0) ? transformedRectangle.Get_Maxima().Y() : transformedRectangle.Get_Minima().Y()
			);
		}
		else {
			return Vector<T, 2>(
				in_point.X(),
				(offset.Y() > 0) ? transformedRectangle.Get_Maxima().Y() : transformedRectangle.Get_Minima().Y()
			);
		}
	}
	else {
		if (yRange.Contains_Inc(in_point.Y())) {
			return Vector<T, 2>(
				(offset.X() > 0) ? transformedRectangle.Get_Maxima().X() : transformedRectangle.Get_Minima().X(),
				in_point.Y()
			);
		}
		else {
			return Vector<T, 2>(
				(offset.X() > 0) ? transformedRectangle.Get_Maxima().X() : transformedRectangle.Get_Minima().X(),
				(offset.Y() > 0) ? transformedRectangle.Get_Maxima().Y() : transformedRectangle.Get_Minima().Y()
			);
		}
	}
}

template<class T>
Vector<T, 2> AxisAlignedRectangleCollisionMask<T>::Get_Closest_Normal(const Vector<T, 2>& in_point) const {
	Vector<T, 2> center = Get_Transformed_Rectangle().Get_Center();
	Vector<T, 2> offset = in_point - center;
	return (abs(offset.X()) > abs(offset.Y())) ? 
		Vector<T, 2>(1, 0) * Sign(offset.X()) : 
		Vector<T, 2>(0, 1) * Sign(offset.Y());
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