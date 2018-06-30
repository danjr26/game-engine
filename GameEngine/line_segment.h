#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

#include "vector.h"

template<class T, uint n>
struct LineSegment {
public:
	Vector<T, n> point1;
	Vector<T, n> point2;

	LineSegment(Vector<T, n>& in_point1, Vector<T, n>& in_point2) :
		point1(in_point1),
		point2(in_point2)
	{}
};

#endif