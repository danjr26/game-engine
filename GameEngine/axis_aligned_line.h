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
	uint Get_Dimension() const;
	T Get_Value() const;
	Vector<T, n> Get_Direction() const;
	void Apply_Transform(const Transform<T, n>& in_transform);

public:
	static AxisAlignedLine<T, n> From_Dimension_Value(uint in_dimension, T in_value);
};

using AxisAlignedLine2f = AxisAlignedLine<float, 2>;
using AxisAlignedLine2d = AxisAlignedLine<double, 2>;
using AxisAlignedLine3f = AxisAlignedLine<float, 3>;
using AxisAlignedLine3d = AxisAlignedLine<double, 3>;

#endif


