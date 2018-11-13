#ifndef AXIS_ALIGNED_HALF_SPACE_H
#define AXIS_ALIGNED_HALF_SPACE_H

#include "transform.h"
#include "exceptions.h"

template<class T, uint n>
class AxisAlignedHalfSpace {
private:
	int dimension;
	T value;

	AxisAlignedHalfSpace(int in_dimension, T in_value);

public:
	uint Get_Dimension() const;
	T Get_Value() const;
	bool Is_Positive() const;
	Vector<T, n> Get_Direction() const;
	void Apply_Transform(Transform<T, n>& in_transform);

public:
	static AxisAlignedHalfSpace<T, n> From_Dimension_Value(uint in_dimension, T in_value, bool in_isPositive);
	static AxisAlignedHalfSpace<T, n> From_Inverse(const AxisAlignedHalfSpace& in_inverse);
};

using AxisAlignedHalfSpace2f = AxisAlignedHalfSpace<float, 2>;
using AxisAlignedHalfSpace2d = AxisAlignedHalfSpace<double, 2>;
using AxisAlignedHalfSpace3f = AxisAlignedHalfSpace<float, 3>;
using AxisAlignedHalfSpace3d = AxisAlignedHalfSpace<double, 3>;


#endif


