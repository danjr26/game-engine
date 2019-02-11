#include "axis_aligned_line.h"
#include "exceptions.h"

template<class T, uint n>
inline AxisAlignedLine<T, n>::AxisAlignedLine(uint in_dimension, T in_value) :
	mDimension(in_dimension),
	mValue(in_value) {

	if (!GEUtil::betwInc(mDimension, (uint)0, (uint)n - 1)) {
		throw InvalidArgumentException();
	}
}

template<class T, uint n>
AxisAlignedLine<T, n>::AxisAlignedLine() :
	mDimension(0),
	mValue(0)
{}

template<class T, uint n>
inline uint AxisAlignedLine<T, n>::getDimension() const {
	return mDimension;
}

template<class T, uint n>
inline T AxisAlignedLine<T, n>::getValue() const {
	return mValue;
}

template<class T, uint n>
inline Vector<T, n> AxisAlignedLine<T, n>::getDirection() const {
	Vector<T, n> out;
	out[mDimension] = GEUtil::sign(mValue);
	return out;
}

template<class T, uint n>
inline void AxisAlignedLine<T, n>::applyTransform(const Transform<T, n>& in_transform) {
	for (Transform<T, n> const* t = &in_transform; t != nullptr; t = t->getConstParent()) {
		mValue *= t->getLocalScale()[mDimension];
		mValue += t->getLocalPosition()[mDimension];
	}
}

template<class T, uint n>
inline AxisAlignedLine<T, n> AxisAlignedLine<T, n>::fromDimensionValue(uint in_dimension, T in_value) {
	return AxisAlignedLine<T, n>(in_dimension, in_value);
}

template class AxisAlignedLine<float, 2>;
template class AxisAlignedLine<double, 2>;
template class AxisAlignedLine<float, 3>;
template class AxisAlignedLine<double, 3>;