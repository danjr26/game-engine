#include "axis_aligned_line.h"
#include "exceptions.h"

template<class T, uint n>
inline AxisAlignedLine<T, n>::AxisAlignedLine(uint in_dimension, T in_value) :
	mDimension(in_dimension),
	mValue(in_value) {

	if (!Between_Inc(mDimension, (uint)0, (uint)n - 1)) {
		throw InvalidArgumentException();
	}
}

template<class T, uint n>
inline uint AxisAlignedLine<T, n>::Get_Dimension() const {
	return mDimension;
}

template<class T, uint n>
inline T AxisAlignedLine<T, n>::Get_Value() const {
	return mValue;
}

template<class T, uint n>
inline Vector<T, n> AxisAlignedLine<T, n>::Get_Direction() const {
	Vector<T, n> out;
	out[mDimension] = Sign(mValue);
	return out;
}

template<class T, uint n>
inline void AxisAlignedLine<T, n>::Apply_Transform(const Transform<T, n>& in_transform) {
	for (Transform<T, n> const* t = &in_transform; t != nullptr; t = t->Get_Const_Parent()) {
		mValue *= t->Get_Local_Scale()[mDimension];
		mValue += t->Get_Local_Position()[mDimension];
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