#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

#include "transform.h"

template<class T, uint n>
class LineSegment {
private:
	Vector<T, n> mPoint1;
	Vector<T, n> mPoint2;

	LineSegment(Vector<T, n>& in_point1, Vector<T, n>& in_point2);

public:
	void Apply_Transform(const Transform<T, n>& in_transform);

	Vector<T, n> Get_Point1() const;
	Vector<T, n> Get_Point2() const;
	Vector<T, n> Get_Center() const;
	Vector<T, n> Get_Offset() const;
	Vector<T, n> Get_Direction() const;
	T Get_Projection_Coefficient1() const;
	T Get_Projection_Coefficient2() const;
	T Get_Projection_Coefficient(const Vector<T, n>& in_point) const;
	Vector<T, n> Get_Projection(const Vector<T, n>& in_point) const;
	T Get_Length();

	Vector<T, n> Random_Point() const;

public:
	static LineSegment<T, n> From_Points(Vector<T, n>& in_point1, Vector<T, n>& in_point2);
	static LineSegment<T, n> From_Point_Offset(Vector<T, n>& in_point, Vector<T, n>& in_offset);
};

using LineSegment2f = LineSegment<float, 2>;
using LineSegment2d = LineSegment<double, 2>;
using LineSegment3f = LineSegment<float, 3>;
using LineSegment3d = LineSegment<double, 3>;

#endif


