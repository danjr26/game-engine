#ifndef AXIS_ALIGNED_HALF_SPACE_H
#define AXIS_ALIGNED_HALF_SPACE_H

#include "transform.h"
#include "exceptions.h"

template<class T, uint n>
class AxisAlignedHalfSpace {
private:
	int dimension;
	T value;

	AxisAlignedHalfSpace(int in_dimension, T in_value) :
		dimension(in_dimension),
		value(in_value) {
		
		if (!Is_Between_Inc(dimension, -(int)n, (int)n - 1)) {
			throw InvalidArgumentException();
		}
	}

public:
	uint Get_Dimension() const {
		return (dimension < 0) ? -(dimension + 1) : dimension;
	}

	T Get_Value() const {
		return value;
	}

	bool Is_Positive() const {
		return dimension >= 0;
	}

	Vector<T, n> Get_Direction() const {
		Vector<T, n> out;
		out[Get_Dimension()] = Sign(value);
		return out;
	}

	void Apply_Transform(Transform& in_transform) {
		for (Transform* t = &in_transform; t != nullptr; t = t->Get_Parent()) {
			value *= t->Get_Local_Scale()[Get_Dimension()];
			value += t->Get_Local_Position()[Get_Dimension()];
		}
	}

	static AxisAlignedHalfSpace<T, n> From_Dimension_Value(uint in_dimension, T in_value, bool in_isPositive) {
		return AxisAlignedHalfSpace<T, n>((in_isPositive) ? (int)in_dimension : -(int)(in_dimension + 1), in_value);
	}

	static AxisAlignedHalfSpace<T, n> From_Inverse(const AxisAlignedHalfSpace& in_inverse) {
		AxisAlignedHalfSpace<T, n> out = in_inverse;
		out.dimension = -(out.dimension + 1);
		return out;
	}
};

using AxisAlignedHalfSpace2f = AxisAlignedHalfSpace<float, 2>;
using AxisAlignedHalfSpace2d = AxisAlignedHalfSpace<double, 2>;
using AxisAlignedHalfSpace3f = AxisAlignedHalfSpace<float, 3>;
using AxisAlignedHalfSpace3d = AxisAlignedHalfSpace<double, 3>;


#endif