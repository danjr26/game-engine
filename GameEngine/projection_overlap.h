#ifndef PROJECTION_OVERLAP_H
#define PROJECTION_OVERLAP_H

#include "vector.h"
#include "range.h"
#include "misc.h"
#include "definitions.h"

template<class T, uint n>
uint findProjectionSeparation(
	Vector<T, n> const* in_points1, uint in_nPoints1, 
	Vector<T, n> const* in_points2, uint in_nPoints2, 
	Vector<T, n> const* in_projectTo, uint in_nProjectTo, 
	Range<T>* out_range = nullptr) {

	if (in_points1 == 0 || in_points2 == 0 || in_nProjectTo == 0) {
		return in_nProjectTo;
	}

	Range<T> range1;
	Range<T> range2;

	for (uint i = 0; i < in_nProjectTo; i++) {
		range1 = Range<T>(in_points1[0].projectionCoeff(in_projectTo[i]));
		for (uint j = 1; j < in_nPoints1; j++) {
			range1.expandTo(in_points1[j].projectionCoeff(in_projectTo[i]));
		}

		range2 = Range<T>(in_points2[0].projectionCoeff(in_projectTo[i]));
		for (uint j = 1; j < in_nPoints2; j++) {
			range2.expandTo(in_points2[j].projectionCoeff(in_projectTo[i]));
		}

		if (out_range == nullptr) {
			if (!range1.intersection(range2)) {
				return i;
			}
		}
		else {
			if (!range1.intersection(range2)) {
				*out_range = (range1.getLow() > range2.getHigh()) ? 
					Range<T>(range1.getLow(), range2.getHigh()) : 
					Range<T>(range1.getHigh(), range2.getLow());
				return i;
			}
		}
	}

	return in_nProjectTo;
}

#endif