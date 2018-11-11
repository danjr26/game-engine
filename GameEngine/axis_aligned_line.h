#ifndef AXIS_ALIGNED_LINE_H
#define AXIS_ALIGNED_LINE_H

#include "vector.h"

template<class T, uint n>
class AxisAlignedLine {
private:
	uint dimension;
	T value;

	AxisAlignedLine(uint in_dimension, T in_value) :
		dimension(in_dimension),
		value(in_value) {

		if (!Is_Between_Inc(dimension, 0, n - 1)) {
			throw InvalidArgumentException();
		}
	}

public:
	uint Get_Dimension() const {
		return dimension;
	}

	T Get_Value() const {
		return value;
	}

	Vector<T, n> Get_Direction() const {
		Vector<T, n> out;
		out[direction] = Sign(value);
		return out;
	}

	void Apply_Transform(Transform<T, n>& in_transform) {
		for (Transform<T, n>* t = &in_transform; t != nullptr; t = t->Get_Parent()) {
			value *= t->Get_Local_Scale()[dimension];
			value += t->Get_Local_Position()[dimension];
		}
	}

	static AxisAlignedLine From_Dimension_Value(uint in_dimension, T in_value) {
		return AxisAlignedLine(in_dimension, in_value);
	}
};

using AxisAlignedLine2f = AxisAlignedLine<float, 2>;
using AxisAlignedLine2d = AxisAlignedLine<double, 2>;
using AxisAlignedLine3f = AxisAlignedLine<float, 3>;
using AxisAlignedLine3d = AxisAlignedLine<double, 3>;

#endif