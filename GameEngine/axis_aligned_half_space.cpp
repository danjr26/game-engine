#include "axis_aligned_half_space.h"

template<class T, uint n>
inline AxisAlignedHalfSpace<T, n>::AxisAlignedHalfSpace(int in_dimension, T in_value) :
	dimension(in_dimension),
	value(in_value) {

	if (!Is_Between_Inc(dimension, -(int)n, (int)n - 1)) {
		throw InvalidArgumentException();
	}
}

template<class T, uint n>
inline uint AxisAlignedHalfSpace<T, n>::Get_Dimension() const {
	return (dimension < 0) ? -(dimension + 1) : dimension;
}

template<class T, uint n>
inline T AxisAlignedHalfSpace<T, n>::Get_Value() const {
	return value;
}

template<class T, uint n>
inline bool AxisAlignedHalfSpace<T, n>::Is_Positive() const {
	return dimension >= 0;
}

template<class T, uint n>
inline Vector<T, n> AxisAlignedHalfSpace<T, n>::Get_Direction() const {
	Vector<T, n> out;
	out[Get_Dimension()] = Sign(value);
	return out;
}

template<class T, uint n>
inline void AxisAlignedHalfSpace<T, n>::Apply_Transform(const Transform<T, n>& in_transform) {
	for (Transform<T, n> const* t = &in_transform; t != nullptr; t = t->Get_Const_Parent()) {
		value *= t->Get_Local_Scale()[Get_Dimension()];
		value += t->Get_Local_Position()[Get_Dimension()];
	}
}

template<class T, uint n>
inline AxisAlignedHalfSpace<T, n> AxisAlignedHalfSpace<T, n>::From_Dimension_Value(uint in_dimension, T in_value, bool in_isPositive) {
	return AxisAlignedHalfSpace<T, n>((in_isPositive) ? (int)in_dimension : -(int)(in_dimension + 1), in_value);
}

template<class T, uint n>
inline AxisAlignedHalfSpace<T, n> AxisAlignedHalfSpace<T, n>::From_Inverse(const AxisAlignedHalfSpace & in_inverse) {
	AxisAlignedHalfSpace<T, n> out = in_inverse;
	out.dimension = -(out.dimension + 1);
	return out;
}

template class AxisAlignedHalfSpace<float, 2>;
template class AxisAlignedHalfSpace<double, 2>;
template class AxisAlignedHalfSpace<float, 3>;
template class AxisAlignedHalfSpace<double, 3>;
