#ifndef AXIS_ALIGNED_HALF_SPACE_H
#define AXIS_ALIGNED_HALF_SPACE_H

#include "transform.h"
#include "exceptions.h"

template<class T, uint n>
class AxisAlignedHalfSpace {
private:
	int mDimension;
	T mValue;

	AxisAlignedHalfSpace(int in_dimension, T in_value);

public:
	uint getDimension() const;
	T getValue() const;
	bool isPositive() const;
	Vector<T, n> getDirection() const;
	void applyTransform(const Transform<T, n>& in_transform);

public:
	static AxisAlignedHalfSpace<T, n> fromDimensionValue(uint in_dimension, T in_value, bool in_isPositive);
	static AxisAlignedHalfSpace<T, n> fromInverse(const AxisAlignedHalfSpace& in_inverse);
};

using AxisAlignedHalfSpace2f = AxisAlignedHalfSpace<float, 2>;
using AxisAlignedHalfSpace2d = AxisAlignedHalfSpace<double, 2>;
using AxisAlignedHalfSpace3f = AxisAlignedHalfSpace<float, 3>;
using AxisAlignedHalfSpace3d = AxisAlignedHalfSpace<double, 3>;


#endif


