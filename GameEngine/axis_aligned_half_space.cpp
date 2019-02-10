#include "axis_aligned_half_space.h"

template<class T, uint n>
inline AxisAlignedHalfSpace<T, n>::AxisAlignedHalfSpace(int in_dimension, T in_value) :
	mDimension(in_dimension),
	mValue(in_value) {

	if (!GEUtil::betwInc(mDimension, -(int)n, (int)n - 1)) {
		throw InvalidArgumentException();
	}
}

template<class T, uint n>
inline uint AxisAlignedHalfSpace<T, n>::getDimension() const {
	return (mDimension < 0) ? -(mDimension + 1) : mDimension;
}

template<class T, uint n>
inline T AxisAlignedHalfSpace<T, n>::getValue() const {
	return mValue;
}

template<class T, uint n>
inline bool AxisAlignedHalfSpace<T, n>::isPositive() const {
	return mDimension >= 0;
}

template<class T, uint n>
inline Vector<T, n> AxisAlignedHalfSpace<T, n>::getDirection() const {
	Vector<T, n> out;
	out[getDimension()] = (isPositive()) ? (T)1 : (T)-1;
	return out;
}

template<class T, uint n>
inline void AxisAlignedHalfSpace<T, n>::applyTransform(const Transform<T, n>& in_transform) {
	for (Transform<T, n> const* t = &in_transform; t != nullptr; t = t->getConstParent()) {
		mValue *= t->getLocalScale()[getDimension()];
		mValue += t->getLocalPosition()[getDimension()];
	}
}

template<class T, uint n>
inline AxisAlignedHalfSpace<T, n> AxisAlignedHalfSpace<T, n>::fromDimensionValue(uint in_dimension, T in_value, bool in_isPositive) {
	return AxisAlignedHalfSpace<T, n>((in_isPositive) ? (int)in_dimension : -(int)(in_dimension + 1), in_value);
}

template<class T, uint n>
inline AxisAlignedHalfSpace<T, n> AxisAlignedHalfSpace<T, n>::fromInverse(const AxisAlignedHalfSpace & in_inverse) {
	AxisAlignedHalfSpace<T, n> out = in_inverse;
	out.mDimension = -(out.mDimension + 1);
	return out;
}

template class AxisAlignedHalfSpace<float, 2>;
template class AxisAlignedHalfSpace<double, 2>;
template class AxisAlignedHalfSpace<float, 3>;
template class AxisAlignedHalfSpace<double, 3>;
