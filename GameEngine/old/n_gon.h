#ifndef N_GON_H
#define N_GON_H

#include "line_segment.h"

template<class T, uint n, uint np>
class NGon {
public:
	Vector<T, n> points[np];

	LineSegment<T, n> Side(uint i) {
		return LineSegment<T, n>(points[i], points[(i + 1) % np]);
	}


};

#endif