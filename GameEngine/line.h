#ifndef LINE_H
#define LINE_H

#include "transform.h"

template<class T, uint n>
class Line {
private:
	Vector<T, n> point;
	Vector<T, n> direction;

	Line(const Vector<T, n>& in_point, const Vector<T, n> in_direction) :
		point(in_point),
		direction(in_direction.Normalized())
	{}

public:
	void Apply_Transform(Transform& in_transform) {
		point = Vector<T, n>(in_transform.Apply_To_Local_Point(Vector<T, 3>(point)));
		direction = Vector<T, n>(in_transform.Apply_To_Local_Direction(Vector<T, 3>(direction)));
	}

	Vector<T, n> Get_Point() const {
		return point;
	}

	Vector<T, n> Get_Direction() const {
		return direction;
	}

	T Get_Projection_Coefficient(const Vector<T, n>& in_point) {
		return (in_point - point).Projection_Coeff(direction) + point;
	}

	Vector<T, n> Get_Projection(const Vector<T, n>& in_point) {
		return (in_point - point).Projection(direction) + point;
	}

	static Line<T, n> From_Points(const Vector<T, n>& in_point1, const Vector<T, n>& in_point2) {
		return Line<T, n>(in_point1, in_point2 - in_point1);
	}

	static Line<T, n> From_Point_Direction(const Vector<T, n>& in_point, const Vector<T, n>& in_direction) {
		return Line<T, n>(in_point, in_direction);
	}
};

using Line2f = Line<float, 2>;
using Line2d = Line<double, 2>;
using Line3f = Line<float, 3>;
using Line3d = Line<double, 3>;

#endif