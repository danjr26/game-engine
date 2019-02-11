#ifndef AXIS_ALIGNED_LINE_H
#define AXIS_ALIGNED_LINE_H

#include "transform.h"

template<class T, uint n>
class AxisAlignedLine {
private:
	uint mDimension;
	T mValue;

	AxisAlignedLine(uint in_dimension, T in_value);

public:
	AxisAlignedLine();

	uint getDimension() const;
	T getValue() const;
	Vector<T, n> getDirection() const;
	void applyTransform(const Transform<T, n>& in_transform);

public:
	static AxisAlignedLine<T, n> fromDimensionValue(uint in_dimension, T in_value);
};

using AxisAlignedLine2f = AxisAlignedLine<float, 2>;
using AxisAlignedLine2d = AxisAlignedLine<double, 2>;
using AxisAlignedLine3f = AxisAlignedLine<float, 3>;
using AxisAlignedLine3d = AxisAlignedLine<double, 3>;

#endif


