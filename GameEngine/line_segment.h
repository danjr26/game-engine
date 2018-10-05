#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

#include "vector.h"

template<class T, uint n>
struct LineSegment {
private:
	Vector<T, n> point1;
	Vector<T, n> point2;

	LineSegment(Vector<T, n>& in_point1, Vector<T, n>& in_point2) :
		point1(in_point1),
		point2(in_point2) 
	{}

public:
	void Apply_Transform(Transform& in_transform) {
		point1 = Vector<T, n>(in_transform.Apply_To_Local_Point(Vector<T, 3>(point1)));
		point2 = Vector<T, n>(in_transform.Apply_To_Local_Point(Vector<T, 3>(point2)));
	}

	Vector<T, n> Get_Point1() const {
		return point1;
	}

	Vector<T, n> Get_Point2() const {
		return point2;
	}

	Vector<T, n> Get_Offset() const {
		return point2 - point1;
	}

	Vector<T, n> Get_Direction() const {
		return (point2 - point1).Normalized();
	}

	T Get_Projection_Coefficient1() const {
		return Get_Direction().Dot(point1);
	}

	T Get_Projection_Coefficient2() const {
		return Get_Direction().Dot(point2);
	}

	T Get_Projection_Coefficient(const Vector<T, n>& in_point) {
		return (in_point - point1).Projection_Coeff(Get_Direction());
	}

	Vector<T, n> Get_Projection(const Vector<T, n>& in_point) {
		return (in_point - point1).Projection(Get_Direction()) + point1;
	}

	static LineSegment<T, n> From_Points(Vector<T, n>& in_point1, Vector<T, n>& in_point2) {
		return LineSegment<T, n>(in_point1, in_point2);
	}

	static LineSegment<T, n> From_Point_Offset(Vector<T, n>& in_point, Vector<T, n>& in_offset) {
		return LineSegment<T, n>(in_point, in_point + in_offset);
	}
};

using LineSegment2f = LineSegment<float, 2>;
using LineSegment2d = LineSegment<double, 2>;
using LineSegment3f = LineSegment<float, 3>;
using LineSegment3d = LineSegment<double, 3>;

#endif