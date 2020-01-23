#include "../include/internal/axis_aligned_half_space.h"

template<class T, uint n>
inline AAHalfSpace<T, n>::AAHalfSpace(int i_dimension, T i_value) :
	mDimension(i_dimension),
	mValue(i_value) {

	if (!betwInc(mDimension, -(int)n, (int)n - 1)) fail();
}

template<class T, uint n>
AAHalfSpace<T, n>::AAHalfSpace() :
	mDimension(0),
	mValue((T)0)
{}

template<class T, uint n>
inline uint AAHalfSpace<T, n>::getDimension() const {
	return (mDimension < 0) ? -(mDimension + 1) : mDimension;
}

template<class T, uint n>
inline T AAHalfSpace<T, n>::getValue() const {
	return mValue;
}

template<class T, uint n>
inline bool AAHalfSpace<T, n>::isPositive() const {
	return mDimension >= 0;
}

template<class T, uint n>
bool AAHalfSpace<T, n>::containsValue(T i_value) const {
	return ceqSwitch(i_value, mValue, mDimension >= 0);
}

template<class T, uint n>
bool AAHalfSpace<T, n>::containsPoint(const Vector<T, n>& in_point) const {
	return containsValue(in_point[getDimension()]);
}

template<class T, uint n>
inline Vector<T, n> AAHalfSpace<T, n>::getDirection() const {
	Vector<T, n> out;
	out[getDimension()] = (isPositive()) ? (T)1 : (T)-1;
	return out;
}

template<class T, uint n>
inline void AAHalfSpace<T, n>::applyTransform(const Transform<T, n>& i_transform) {
	for (Transform<T, n> const* t = &i_transform; t != nullptr; t = t->getConstParent()) {
		mValue *= t->getLocalScale()[getDimension()];
		mValue += t->getLocalPosition()[getDimension()];
	}
}

template<class T, uint n>
void AAHalfSpace<T, n>::getClosest(ClosestRequest<T, n>& io_request) const {
	if (io_request.wantsPoint()) {
		io_request.mPoint[getDimension()] = mValue;
	}

	if (io_request.wantsNormal()) {
		io_request.mNormal = -getDirection();
	}

	if (n == 2) {
		io_request.mContactType = ClosestRequest<T, n>::on_edge;
	}
	else if (n == 3) {
		io_request.mContactType = ClosestRequest<T, n>::on_face;
	}
}

template<class T, uint n>
inline AAHalfSpace<T, n> AAHalfSpace<T, n>::fromDimensionValue(uint i_dimension, T i_value, bool i_isPositive) {
	return AAHalfSpace<T, n>((i_isPositive) ? (int)i_dimension : -(int)(i_dimension + 1), i_value);
}

template<class T, uint n>
inline AAHalfSpace<T, n> AAHalfSpace<T, n>::fromInverse(const AAHalfSpace & i_inverse) {
	AAHalfSpace<T, n> out = i_inverse;
	out.mDimension = -(out.mDimension + 1);
	return out;
}

template class AAHalfSpace<float, 2>;
template class AAHalfSpace<double, 2>;
template class AAHalfSpace<float, 3>;
template class AAHalfSpace<double, 3>;
