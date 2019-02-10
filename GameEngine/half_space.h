#ifndef HALF_SPACE_H
#define HALF_SPACE_H

#include "transform.h"

template<class T, uint n>
class HalfSpace {
private:
	Vector<T, n> mPoint;
	Vector<T, n> mDirection;

	HalfSpace(const Vector<T, n>& in_point, const Vector<T, n>& in_direction);

public:
	void applyTransform(const Transform<T, n>& in_transform);
	Vector<T, n> getPoint() const;
	void setPoint(const Vector<T, n>& in_point);
	Vector<T, n> getDirection() const;
	void setDirection(const Vector<T, n>& in_direction);
	T getProjectionCoefficient() const;
	Vector<T, n> getProjection() const;

public:
	static HalfSpace<T, n> fromPointDirection(const Vector<T, n>& in_point, const Vector<T, n>& in_direction);
};

using HalfSpace2f = HalfSpace<float, 2>;
using HalfSpace2d = HalfSpace<double, 2>;
using HalfSpace3f = HalfSpace<float, 3>;
using HalfSpace3d = HalfSpace<double, 3>;

#endif


