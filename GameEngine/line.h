#ifndef LINE_H
#define LINE_H

#include "transform.h"

template<class T, uint n>
class Line {
private:
	Vector<T, n> point;
	Vector<T, n> direction;

private:
	Line(const Vector<T, n>& in_point, const Vector<T, n> in_direction);

public:
	void Apply_Transform(Transform<T, n>& in_transform);
	Vector<T, n> Get_Point() const;
	Vector<T, n> Get_Direction() const;
	T Get_Projection_Coefficient() const;
	T Get_Projection_Coefficient(const Vector<T, n>& in_point) const;
	Vector<T, n> Get_Projection(const Vector<T, n>& in_point) const;

	T Distance_To(const Vector<T, n>& in_point) const;
	Vector<T, n> Flip(const Vector<T, n>& in_point) const;

public:
	static Line<T, n> From_Points(const Vector<T, n>& in_point1, const Vector<T, n>& in_point2);
	static Line<T, n> From_Point_Direction(const Vector<T, n>& in_point, const Vector<T, n>& in_direction);
};

using Line2f = Line<float, 2>;
using Line2d = Line<double, 2>;
using Line3f = Line<float, 3>;
using Line3d = Line<double, 3>;

#endif


