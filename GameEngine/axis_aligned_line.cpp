#include "axis_aligned_line.h"
#include "exceptions.h"

template<class T, uint n>
inline AxisAlignedLine<T, n>::AxisAlignedLine(uint in_dimension, T in_value) :
	dimension(in_dimension),
	value(in_value) {

	if (!Is_Between_Inc(dimension, (uint)0, (uint)n - 1)) {
		throw InvalidArgumentException();
	}
}

template<class T, uint n>
inline uint AxisAlignedLine<T, n>::Get_Dimension() const {
	return dimension;
}

template<class T, uint n>
inline T AxisAlignedLine<T, n>::Get_Value() const {
	return value;
}

template<class T, uint n>
inline Vector<T, n> AxisAlignedLine<T, n>::Get_Direction() const {
	Vector<T, n> out;
	out[dimension] = Sign(value);
	return out;
}

template<class T, uint n>
inline void AxisAlignedLine<T, n>::Apply_Transform(Transform<T, n>& in_transform) {
	for (Transform<T, n>* t = &in_transform; t != nullptr; t = t->Get_Parent()) {
		value *= t->Get_Local_Scale()[dimension];
		value += t->Get_Local_Position()[dimension];
	}
}

template<class T, uint n>
inline AxisAlignedLine<T, n> AxisAlignedLine<T, n>::From_Dimension_Value(uint in_dimension, T in_value) {
	return AxisAlignedLine<T, n>(in_dimension, in_value);
}

template class AxisAlignedLine<float, 2>;
template class AxisAlignedLine<double, 2>;
template class AxisAlignedLine<float, 3>;
template class AxisAlignedLine<double, 3>;