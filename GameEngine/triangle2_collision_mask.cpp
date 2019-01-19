#include "triangle2_collision_mask.h"

template<class T>
Triangle2CollisionMask<T>::Triangle2CollisionMask(const Triangle<T, 2>& in_triangle, bool in_ignoreTransform) :
	CollisionMask<T, 2>(in_ignoreTransform),
	triangle(in_triangle)
{}

template<class T>
Triangle<T, 2>& Triangle2CollisionMask<T>::Get_Triangle() {
	return triangle;
}

template<class T>
Triangle<T, 2> Triangle2CollisionMask<T>::Get_Transformed_Triangle() const {
	if (ignoreTransform) {
		return triangle;
	}
	else {
		Triangle<T, 2> out = triangle;
		out.Apply_Transform(transform);
		return out;
	}
}

template<class T>
void Triangle2CollisionMask<T>::Apply_Transform() {
	triangle.Apply_Transform(transform);
}

template<class T>
Triangle2CollisionMask<T>* Triangle2CollisionMask<T>::Clone() const {
	return new Triangle2CollisionMask<T>(*this);
}

template<class T>
Vector<T, 2> Triangle2CollisionMask<T>::Get_Closest_Point(const Vector<T, 2>& in_point) const {
	auto transformedTriangle = Get_Transformed_Triangle();
	
	Vector<T, 2> corners[3];
	transformedTriangle.Get_Points(corners);

	bool alley[3];
	bool front[3];

	throw NotImplementedException();
	return Vector<T, 2>();
}

template<class T>
Vector<T, 2> Triangle2CollisionMask<T>::Get_Closest_Normal(const Vector<T, 2>& in_point, PointNormalPolicy in_policy) const {
	auto transformedTriangle = Get_Transformed_Triangle();

	Vector<T, 2> corners[3];
	Vector<T, 2> normals[3];
	transformedTriangle.Get_Normals(normals);
	transformedTriangle.Get_Points(corners);
	Vector<T, 2> cornerNormals[3];
	Vector<T, 2> cornerTangents[3];
	bool slice[3];
	bool alley[3];
	bool front[3];
	for (uint i = 0; i < 3; i++) {
		cornerNormals[i] = (normals[i] + normals[(i + 2) % 3]) / 2;
		cornerTangents[i] = cornerNormals[i].Orthogonal();

		slice[i] = cornerTangents[i].Dot(in_point) >= cornerTangents[i].Dot(corners[i]);

		Vector<T, 2> offset = corners[(i + 1) % 3] - corners[i];
		alley[i] = Is_Between_Exc(
			in_point.Dot(offset),
			corners[i].Dot(offset),
			corners[(i + 1) % 3].Dot(offset)
		);

		front[i] = (in_point - corners[i]).Dot(normals[i]) >= 0.0;

		if (front[i] && alley[i]) {
			return normals[i];
		}
	}

	if (!front[0] && !front[1] && !front[2]) {
		for (uint i = 0; i < 3; i++) {
			if (slice[i] && !slice[(i + 1) % 3]) {
				return normals[i];
			}
		}
		throw ProcessFailureException();
	}

	switch (in_policy) {
	case PointNormalPolicy::zero:
		return Vector<T, 2>();
		break;
	case PointNormalPolicy::nearest_edge: {
		for (uint i = 0; i < 3; i++) {
			if (slice[i] && !slice[(i + 1) % 3]) {
				return normals[i];
			}
		}
		throw ProcessFailureException();
	}
		break;
	case PointNormalPolicy::towards_point: {
		uint index = Min_Index({
			(in_point - corners[0]).Dot_Self(),
			(in_point - corners[1]).Dot_Self(),
			(in_point - corners[2]).Dot_Self()
		});

		return (in_point - corners[index]).Normalized();
	}
		break;
	default:
		throw InvalidArgumentException();
	}
}

template<class T>
Collision<T, 2> Triangle2CollisionMask<T>::Accept_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CollisionMask<T, 2>& in_other) {
	return in_other.Accept_Secondhand_Evaluator(in_evaluator, *this);
}

template<class T>
Collision<T, 2> Triangle2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedHalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Triangle2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedLine2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Triangle2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, AxisAlignedRectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Triangle2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, CircleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Triangle2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, HalfSpace2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Triangle2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Line2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Triangle2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, LineSegment2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Triangle2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Mesh2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Triangle2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Point2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Triangle2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Ray2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Triangle2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, RectangleCollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template<class T>
Collision<T, 2> Triangle2CollisionMask<T>::Accept_Secondhand_Evaluator(CollisionEvaluator<T, 2>& in_evaluator, Triangle2CollisionMask<T>& in_other) {
	return in_evaluator.Evaluate_Typed(in_other, *this);
}

template class Triangle2CollisionMask<float>;
template class Triangle2CollisionMask<double>;