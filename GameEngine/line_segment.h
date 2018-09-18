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
	static LineSegment<T, n> From_Points(Vector<T, n>& in_point1, Vector<T, n>& in_point2) {
		return LineSegment<T, n>(in_point1, in_point2);
	}
};

using LineSegment2f = LineSegment<float, 2>;
using LineSegment2d = LineSegment<double, 2>;
using LineSegment3f = LineSegment<float, 3>;
using LineSegment3d = LineSegment<double, 3>;

#endif