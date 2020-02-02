#include "../include/internal/projection.h"

template<class T>
Projection<T>::Projection() :
	mNear(0),
	mFar(0),
	mNearDimen(),
	mType(ProjectionType::undefined)
{}

template<class T>
Matrix<T, 4, 4> Projection<T>::getMatrix() const {
	T out[4][4] = {};
	switch (mType) {
	case ProjectionType::perspective:
		out[0][0] = mNear / mNearDimen.x();
		out[1][1] = mNear / mNearDimen.y();
		out[2][2] = -(mFar + mNear) / (mFar - mNear);
		out[3][2] = (T)(-1);
		out[2][3] = (T)(-2) * mFar * mNear / (mFar - mNear);
		break;
	case ProjectionType::orthographic:
		out[0][0] = (T)(1) / mNearDimen.x();
		out[1][1] = (T)(1) / mNearDimen.y();
		out[2][2] = (T)(-2) / (mFar - mNear);
		out[3][3] = (T)(1);
		out[2][3] = -(mFar + mNear) / (mFar - mNear);
	default: fail();
	}
	return out;
}

template struct Projection<float>;
template struct Projection<double>;
